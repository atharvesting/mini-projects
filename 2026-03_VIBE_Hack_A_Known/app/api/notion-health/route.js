import { NextResponse } from 'next/server';
import { Client } from '@notionhq/client';

function getSecretFromRequest(request) {
  const authHeader = request.headers.get('authorization');
  const bearer = authHeader?.startsWith('Bearer ') ? authHeader.slice(7) : null;
  const querySecret = new URL(request.url).searchParams.get('secret');
  return bearer || querySecret;
}

function normalizeName(page) {
  return page.properties?.Name?.title?.[0]?.plain_text ?? '(untitled)';
}

export async function GET(request) {
  const secret = process.env.REVALIDATE_SECRET;
  const provided = getSecretFromRequest(request);

  if (!secret) {
    return NextResponse.json(
      { ok: false, message: 'Missing REVALIDATE_SECRET on server.' },
      { status: 500 }
    );
  }

  if (provided !== secret) {
    return NextResponse.json({ ok: false, message: 'Unauthorized.' }, { status: 401 });
  }

  const notionToken = process.env.NOTION_TOKEN;
  const dataSourceId = process.env.NOTION_DATA_SOURCE_ID ?? process.env.NOTION_DATABASE_ID;

  if (!notionToken || !dataSourceId) {
    return NextResponse.json(
      {
        ok: false,
        message: 'Missing NOTION_TOKEN and/or NOTION_DATA_SOURCE_ID (or NOTION_DATABASE_ID).',
        env: {
          hasNotionToken: Boolean(notionToken),
          hasDataSourceId: Boolean(dataSourceId),
        },
      },
      { status: 500 }
    );
  }

  try {
    const notion = new Client({ auth: notionToken, timeoutMs: 10000 });
    const response = await notion.dataSources.query({ data_source_id: dataSourceId, page_size: 10 });

    return NextResponse.json({
      ok: true,
      env: {
        hasNotionToken: true,
        hasDataSourceId: true,
      },
      dataSourceIdPreview: `${dataSourceId.slice(0, 8)}...${dataSourceId.slice(-4)}`,
      countSample: response.results.length,
      sampleNames: response.results.map(normalizeName),
      hasMore: response.has_more,
      nextCursor: response.next_cursor,
    });
  } catch (error) {
    return NextResponse.json(
      {
        ok: false,
        message: error?.message ?? 'Unknown Notion API error',
        code: error?.code ?? 'unknown_error',
      },
      { status: 500 }
    );
  }
}
