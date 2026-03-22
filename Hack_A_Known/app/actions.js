'use server';

import { Client } from '@notionhq/client';

function normalizeKey(value) {
  return String(value ?? '')
    .trim()
    .toLowerCase()
    .replace(/[\s_-]+/g, '');
}

function findPropertyKey(schema, candidates, expectedType) {
  if (!schema) return null;
  const normalizedCandidates = candidates.map(normalizeKey);

  for (const [key, property] of Object.entries(schema)) {
    const keyMatch = normalizedCandidates.includes(normalizeKey(key));
    if (!keyMatch) continue;
    if (expectedType && property?.type !== expectedType) continue;
    return key;
  }

  return null;
}

function setRichText(properties, key, value) {
  if (!key || !value) return;
  properties[key] = {
    rich_text: [{ text: { content: value } }],
  };
}

export async function suggestHackathon(formData) {
  const name = String(formData.get('name') ?? '').trim();
  const url = String(formData.get('url') ?? '').trim();
  const startDate = String(formData.get('startDate') ?? '').trim();
  const endDate = String(formData.get('endDate') ?? '').trim();
  const venue = String(formData.get('venue') ?? '').trim();
  const description = String(formData.get('description') ?? '').trim();
  const username = String(formData.get('username') ?? '').trim();

  if (!name || !url) {
    return { error: 'Name and URL are required.' };
  }

  if (!endDate) {
    return { error: 'End date is required for suggestions.' };
  }

  const today = new Date();
  today.setHours(0, 0, 0, 0);
  const parsedEndDate = new Date(`${endDate}T00:00:00`);

  if (Number.isNaN(parsedEndDate.getTime())) {
    return { error: 'Please select a valid end date.' };
  }

  if (parsedEndDate < today) {
    return { error: 'Suggestions for hackathons conducted in the past are not allowed.' };
  }

  const token = process.env.NOTION_TOKEN;
  const dataSourceId = process.env.NOTION_DATA_SOURCE_ID;
  const databaseId = process.env.NOTION_DATABASE_ID;
  const parentId = dataSourceId ?? databaseId;

  if (!token || !parentId) {
    return { error: 'Database configuration missing. Try again later.' };
  }

  const notion = new Client({ auth: token });

  try {
    const parent = dataSourceId
      ? { data_source_id: dataSourceId }
      : { database_id: databaseId };

    const container = dataSourceId
      ? await notion.dataSources.retrieve({ data_source_id: dataSourceId })
      : await notion.databases.retrieve({ database_id: databaseId });
    const schema = container?.properties ?? {};

    const titleKey = findPropertyKey(schema, ['name', 'title'], 'title');
    const urlKey = findPropertyKey(schema, ['url', 'website', 'link', 'registration link'], 'url');
    const pendingKey = findPropertyKey(schema, ['pending'], 'checkbox');
    const rangeDateKey = findPropertyKey(schema, ['duration', 'date', 'dates', 'timeline'], 'date');
    const startDateKey = findPropertyKey(schema, ['start date', 'startdate', 'start'], 'date');
    const endDateKey = findPropertyKey(schema, ['end date', 'enddate', 'end', 'deadline'], 'date');
    const venueKey = findPropertyKey(schema, ['venue', 'location'], 'rich_text');
    const descriptionKey = findPropertyKey(schema, ['description', 'details', 'summary'], 'rich_text');
    const usernameKey = findPropertyKey(schema, ['username', 'contributor', 'contributor name'], 'rich_text');

    if (!titleKey || !urlKey) {
      return {
        error:
          'Could not find required Notion properties for title/url. Please ensure your data source has title and url properties (for example: name + url).',
      };
    }

    const properties = {
      [titleKey]: {
        title: [{ text: { content: name } }],
      },
      [urlKey]: { url },
    };

    if (pendingKey) {
      properties[pendingKey] = { checkbox: true };
    }

    if (rangeDateKey && (startDate || endDate)) {
      properties[rangeDateKey] = {
        date: {
          start: startDate || endDate,
          ...(startDate && endDate ? { end: endDate } : {}),
        },
      };
    } else {
      if (startDateKey && startDate) {
        properties[startDateKey] = { date: { start: startDate } };
      }
      if (endDateKey && endDate) {
        properties[endDateKey] = { date: { start: endDate } };
      }
    }

    setRichText(properties, venueKey, venue);
    setRichText(properties, descriptionKey, description);
    setRichText(properties, usernameKey, username);

    await notion.pages.create({
      parent,
      properties,
    });

    return { success: true };
  } catch (err) {
    if (err?.code === 'restricted_resource') {
      return {
        error:
          'Notion integration lacks write access to this data source/database. Open the database in Notion -> Connections, add your integration, and ensure integration capability "Insert content" is enabled.',
      };
    }

    console.error('Error creating suggestion:', err);
    return { error: 'Failed to submit the suggestion. Please try again later.' };
  }
}
