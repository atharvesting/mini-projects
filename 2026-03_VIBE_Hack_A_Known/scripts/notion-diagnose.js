const fs = require('fs')
const path = require('path')
const { Client } = require('@notionhq/client')

function loadEnvLocal() {
  const envPath = path.join(process.cwd(), '.env.local')
  if (!fs.existsSync(envPath)) return

  const lines = fs.readFileSync(envPath, 'utf8').split(/\r?\n/)
  for (const line of lines) {
    const trimmed = line.trim()
    if (!trimmed || trimmed.startsWith('#')) continue

    const eq = trimmed.indexOf('=')
    if (eq === -1) continue

    const key = trimmed.slice(0, eq).trim()
    let value = trimmed.slice(eq + 1).trim()

    if (
      (value.startsWith('"') && value.endsWith('"')) ||
      (value.startsWith("'") && value.endsWith("'"))
    ) {
      value = value.slice(1, -1)
    }

    if (!process.env[key]) process.env[key] = value
  }
}

async function main() {
  loadEnvLocal()

  if (!process.env.NOTION_TOKEN) {
    console.log('Missing NOTION_TOKEN in environment')
    process.exit(1)
  }

  const notion = new Client({ auth: process.env.NOTION_TOKEN, timeoutMs: 10000 })

  console.log('NOTION_DATABASE_ID present:', Boolean(process.env.NOTION_DATABASE_ID))

  const dataSources = await notion.search({
    filter: { property: 'object', value: 'data_source' },
    page_size: 20,
  })

  console.log('Accessible data sources:', dataSources.results.length)
  for (const source of dataSources.results) {
    const title = (source.title || []).map((t) => t.plain_text).join(' ') || '(untitled)'
    console.log(`- ${source.id} | ${title}`)
  }

  if (!process.env.NOTION_DATABASE_ID) return

  try {
    const queryResult = await notion.dataSources.query({
      data_source_id: process.env.NOTION_DATABASE_ID,
      page_size: 10,
    })
    console.log('Configured NOTION_DATABASE_ID works as data_source_id ✅')
    console.log('Rows returned in sample query:', queryResult.results.length)

    if (queryResult.results.length) {
      const names = queryResult.results.map((page) => {
        return page.properties?.Name?.title?.[0]?.plain_text ?? '(untitled)'
      })
      console.log('Sample names:', names.join(' | '))
    }
  } catch (error) {
    console.log('Configured NOTION_DATABASE_ID failed as data_source_id ❌')
    console.log(`${error.code}: ${error.message}`)
  }
}

main().catch((error) => {
  console.error('Diagnostic failed:', error.message)
  process.exit(1)
})
