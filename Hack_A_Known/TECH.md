# TECH.md — Technical Architecture Reference

## Stack Decision Log

### Next.js 15 — App Router, JavaScript

- App Router is used because it enables React Server Components, which is critical for keeping the Notion token server-side only and never leaking it to the browser bundle.
- JavaScript (not TypeScript) keeps bootstrapping fast and removes transpilation complexity overhead.
- ISR is implemented via `export const revalidate = 3600` at the route segment level, or via `unstable_cache` in data-fetching functions when the SDK bypasses native `fetch`.

---

### Notion as CMS

- **SDK version:** `@notionhq/client` v5 (current as of March 2026)
- **API version targeted:** `2025-09-03` (set automatically by SDK v5)
- **Endpoint used:** `notion.dataSources.query({ data_source_id: DATABASE_ID })`

> ⚠️ **Breaking change note:** In SDK v4 and API version `2022-06-28`, this was `notion.databases.query({ database_id: DATABASE_ID })`. These are NOT interchangeable. Do not mix SDK versions or API versions.

- **Pagination:** Each query returns up to 100 results. Use `start_cursor` in a loop if >100 hackathons are expected.
- **Rate limit:** 3 requests/second (average) per integration. SDK retries automatically on HTTP 429 with exponential backoff. Configurable via `{ retry: { maxRetries: 5 } }` in the client options.
- **Timeout:** Set `timeoutMs: 10000` on the client constructor to prevent build hangs when Notion's API is slow during static generation.
- **Caching strategy:** `unstable_cache` from `next/cache` wraps the SDK call because the SDK uses its own internal HTTP client — not native `fetch` — so Next.js cannot cache it automatically at the fetch layer.

---

### Gantt View — Custom CSS Timeline (No Library)

`gantt-task-react` was evaluated and **rejected**:

| Issue | Detail |
| :--- | :--- |
| Abandoned | Last published **4 years ago** (2021), targeting React 16/17 |
| React 18/19 incompatible | Uses direct DOM manipulation that breaks concurrent rendering |
| SSR unsafe | Crashes Next.js App Router builds (uses `document`/`window` without guards) |
| Unmaintained | 200+ open issues, no maintainer activity |

**Replacement approach:** A custom `HackathonGantt.jsx` component built with Tailwind CSS and `date-fns`.

**Bar positioning logic:**

```js
const minDate = subDays(min(hackathons.map(h => parseISO(h.startDate))), 7)
const maxDate = addDays(max(hackathons.map(h => parseISO(h.endDate))), 7)
const totalDays = differenceInDays(maxDate, minDate)

// For each hackathon bar:
const leftPercent = (differenceInDays(parseISO(h.startDate), minDate) / totalDays) * 100
const widthPercent = (differenceInDays(parseISO(h.endDate), parseISO(h.startDate)) / totalDays) * 100
```

**Status color mapping:**

| Status | Tailwind Classes |
| :--- | :--- |
| `past` | `bg-gray-300 text-gray-500` |
| `ongoing` | `bg-emerald-500 text-white` |
| `future` | `bg-blue-200 text-blue-800 border border-blue-400` |

- "Today" indicator: a vertical `w-px bg-red-400` line positioned at `((differenceInDays(today, minDate)) / totalDays) * 100`% from left, spanning full height.
- Mobile: outer wrapper uses `overflow-x-auto` with a min-width on the inner timeline to enable horizontal scrolling.

---

### Tailwind CSS v4

> Do not use v3 patterns. The upgrade tool is `npx @tailwindcss/upgrade`.

**Critical differences from v3:**

| Topic | v3 Pattern | v4 Pattern |
| :--- | :--- | :--- |
| PostCSS plugin package | `tailwindcss` | `@tailwindcss/postcss` |
| CSS entry directive | `@tailwind base/components/utilities` | `@import "tailwindcss"` |
| Font theme injection | `tailwind.config.js` → `theme.extend.fontFamily` | `@theme { --font-sans: ...; }` in `globals.css` |
| Config file detection | Auto-loaded from project root | Must use `@config` directive or CSS-first config |
| `shadow-sm` | Small shadow | Renamed to `shadow-xs`; `shadow-sm` is now medium |
| `rounded-sm` | Small radius | Renamed to `rounded-xs` |
| `ring` | 3px blue ring | 1px currentColor; use `ring-3 ring-blue-500` for old behavior |
| `outline-none` | Hides outline | Renamed to `outline-hidden`; `outline-none` now sets `none` literally |
| Default border color | `gray-200` | `currentColor` — must add `border-gray-200` explicitly |
| `hover:` variant | Always fires on hover | Only fires when `@media (hover: hover)` — test on touch devices |
| `space-y-*` selector | Targets `:not([hidden])` siblings | Changed; migrate to `flex` + `gap-*` |

`postcss.config.mjs`:
```js
export default {
  plugins: {
    '@tailwindcss/postcss': {},
  },
}
```

`app/globals.css`:
```css
@import "tailwindcss";

@theme {
  --font-sans: var(--font-geist-sans), sans-serif;
}
```

**Browser support floor (v4):** Safari 16.4+, Chrome 111+, Firefox 128+.

---

### shadcn/ui — JavaScript Mode

`components.json` critical fields:

```json
{
  "style": "new-york",
  "tsx": false,
  "rsc": true,
  "tailwind": {
    "config": "",
    "css": "app/globals.css",
    "baseColor": "zinc",
    "cssVariables": true
  },
  "aliases": {
    "components": "@/components",
    "utils": "@/lib/utils"
  }
}
```

- `"tsx": false` → CLI generates `.jsx` files instead of `.tsx`
- `"rsc": true` → CLI auto-inserts `"use client"` on interactive components
- `"tailwind.config": ""` → required for Tailwind v4 (no config file to reference)

---

### Vercel Deployment + ISR

- ISR is available on all tiers, including Hobby (free).
- Set `revalidate: 3600` (1 hour) minimum on Hobby to stay within function invocation limits.
- On-demand revalidation: call `revalidateTag('hackathons')` from a protected Route Handler if an immediate refresh after a Notion edit is needed.
- ISR cache persists across deployments. On revalidation failure, stale content continues to be served (graceful degradation).
- Debug ISR locally: add `NEXT_PRIVATE_DEBUG_CACHE=1` to `.env.local` to log cache hits/misses in the terminal.

**Environment Variable Rules:**

| Variable | Prefix | Exposure |
| :--- | :--- | :--- |
| `NOTION_TOKEN` | None | Server-only (never in browser bundle) |
| `NOTION_DATABASE_ID` | None | Server-only |
| `NEXT_PUBLIC_*` | `NEXT_PUBLIC_` | Inlined at build time into JS bundle — requires a rebuild to update |

> `NOTION_TOKEN` must **never** have the `NEXT_PUBLIC_` prefix.

---

## Normalized Data Shape Contract

All components receive hackathon data in this shape:

```js
{
  id: string,           // Notion page UUID
  name: string,         // Title property plain text
  startDate: string,    // ISO 8601 date string, e.g. "2026-04-01"
  endDate: string,      // ISO 8601 date string, e.g. "2026-04-03"
  tags: string[],       // Array of multi-select names
  status: string|null,  // Manual override from Notion Select, or null
  url: string|null,     // Registration page URL
  description: string,  // Plain text description for the drawer
}
```

---

## Notion Database Setup

**Required properties (exact names — the normalization function depends on these):**

| Property Name | Notion Type | Notes |
| :--- | :--- | :--- |
| `Name` | Title | Required — main identifier |
| `Duration` | Date | Enable "End date" toggle in Notion |
| `Tags` | Multi-select | Free-form; e.g. AI, Web3, Beginner, Online |
| `Status` | Select | Suggested options: Active, Upcoming, Closed |
| `URL` | URL | Direct registration link |
| `Description` | Text (rich text) | Shown in the detail drawer |
