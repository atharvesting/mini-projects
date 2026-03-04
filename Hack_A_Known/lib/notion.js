import { unstable_cache } from 'next/cache';
import { Client } from '@notionhq/client';

const notion = new Client({
  auth: process.env.NOTION_TOKEN,
  timeoutMs: 10000, // Prevent build hangs if Notion API is slow
});

const notionDataSourceId =
  process.env.NOTION_DATA_SOURCE_ID ?? process.env.NOTION_DATABASE_ID;

function getPropertyText(property) {
  if (!property) return null;

  if (property.type === 'rich_text') {
    return property.rich_text?.map((item) => item.plain_text).join(' ')?.trim() || null;
  }

  if (property.type === 'title') {
    return property.title?.map((item) => item.plain_text).join(' ')?.trim() || null;
  }

  if (property.type === 'formula') {
    return property.formula?.string ?? null;
  }

  return null;
}

function extractIsoDates(text) {
  if (!text) return [];
  return text.match(/\d{4}-\d{2}-\d{2}/g) ?? [];
}

function extractDateFromProperty(property, mode = 'start') {
  if (!property) return null;

  if (property.type === 'date') {
    if (mode === 'start') return property.date?.start ?? null;
    return property.date?.end ?? property.date?.start ?? null;
  }

  const text = getPropertyText(property);
  const [start, end] = extractIsoDates(text);

  if (mode === 'start') return start ?? null;
  return end ?? start ?? null;
}

function pickDate(properties, keys, mode) {
  for (const key of keys) {
    const value = extractDateFromProperty(properties[key], mode);
    if (value) return value;
  }
  return null;
}

function normalizeHackathon(page) {
  const properties = page.properties || {};

  const titleProperty = Object.values(properties).find(
    (property) => property?.type === 'title'
  );

  const startDate =
    pickDate(properties, ['Duration', 'Start Date', 'StartDate', 'startDate'], 'start');

  const endDate =
    pickDate(properties, ['Duration', 'End Date', 'EndDate', 'endDate'], 'end');

  return {
    id: page.id,
    name: properties.Name?.title?.[0]?.plain_text ?? titleProperty?.title?.[0]?.plain_text ?? 'Untitled',
    startDate,
    endDate,
    tags: properties.Tags?.multi_select?.map((t) => t.name) ?? [],
    url: properties.URL?.url ?? null,
    description: properties.Description?.rich_text?.[0]?.plain_text ?? '',
  };
}

export const getHackathonsData = unstable_cache(
  async () => {
    if (!process.env.NOTION_TOKEN || !notionDataSourceId) {
      console.error('Missing NOTION_TOKEN and/or NOTION_DATA_SOURCE_ID (or NOTION_DATABASE_ID) environment variable.');
      return { items: [], fetchedAt: null };
    }

    try {
      const allResults = [];
      let cursor = undefined;

      do {
        const response = await notion.dataSources.query({
          data_source_id: notionDataSourceId,
          start_cursor: cursor,
          page_size: 100,
        });

        allResults.push(...response.results);
        cursor = response.has_more ? response.next_cursor : undefined;
      } while (cursor);

      return {
        items: allResults.map(normalizeHackathon),
        fetchedAt: new Date().toISOString(),
      };
    } catch (error) {
      console.error('Error fetching Notion data:', error?.message ?? 'Unknown error');
      return { items: [], fetchedAt: null };
    }
  },
  ['hackathons'],
  { revalidate: 120, tags: ['hackathons'] }
);

export async function getHackathons() {
  const data = await getHackathonsData();
  return data.items;
}
