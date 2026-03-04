# TASKS.md — Phased Build Checklist

Track progress by checking off items as you complete them. Follow the phases in order — later phases depend on earlier ones.

---

## Phase 1: Foundation

**Goal:** A deployable, empty Next.js 15 app with Tailwind v4 and shadcn/ui configured correctly.

- [ ] **Init Next.js project (JavaScript, no TypeScript)**
  ```bash
  npx create-next-app@latest hack-a-known \
    --no-typescript \
    --tailwind \
    --eslint \
    --app \
    --no-src-dir \
    --import-alias "@/*"
  cd hack-a-known
  ```

- [ ] **Fix Tailwind v4 PostCSS config** — replace the full contents of `postcss.config.mjs`:
  ```js
  export default {
    plugins: {
      '@tailwindcss/postcss': {},
    },
  }
  ```
  Remove `autoprefixer` from this file and from `package.json` devDependencies if it was added.

- [ ] **Fix `app/globals.css`** — replace the three `@tailwind` directives with a single import:
  ```css
  @import "tailwindcss";
  ```
  Keep any `@theme {}` or `:root {}` blocks that follow.

- [ ] **Initialize shadcn/ui**
  ```bash
  npx shadcn@latest init
  ```
  Answer the wizard:
  - Style → `new-york`
  - Base color → `zinc`
  - CSS variables → `yes`

- [ ] **Set `"tsx": false` in `components.json`** — open the generated `components.json` and change `"tsx": true` to `"tsx": false`. Also confirm `"rsc": true` is set. For Tailwind v4, set `"tailwind.config": ""` (empty string).

- [ ] **Install required shadcn components**
  ```bash
  npx shadcn@latest add table sheet badge tabs button
  ```

- [ ] **Install utility packages**
  ```bash
  npm install date-fns @notionhq/client
  ```

- [ ] **Create `.env.local`** by copying `.env.example` and filling in your values (token and database ID come in Phase 2).

- [ ] **Enable Vercel Web Analytics**
  ```bash
  npm install @vercel/analytics
  ```
  In `app/layout.js`, import and add `<Analytics />` from `@vercel/analytics/react` inside the `<body>` tag.

- [ ] **Create Vercel project**
  ```bash
  npx vercel link
  ```
  Or connect via the Vercel dashboard. Add `NOTION_TOKEN` and `NOTION_DATABASE_ID` env vars in Vercel Dashboard → Settings → Environment Variables. (Placeholder values for now — real values added in Phase 2.)

- [ ] **Verify clean build**
  ```bash
  npm run build
  ```
  Build must complete with zero errors before moving to Phase 2.

---

## Phase 2: Data Layer

**Goal:** Live Notion data flowing into the app at build time, normalized and cached.

- [ ] **Create Notion database** with exact property names and types from TECH.md:
  - `Name` (Title)
  - `Duration` (Date — enable the "End date" toggle)
  - `Tags` (Multi-select)
  - `Status` (Select — add options: Active, Upcoming, Closed)
  - `URL` (URL)
  - `Description` (Text / Rich text)

- [ ] **Create Notion integration**
  1. Go to https://www.notion.so/my-integrations
  2. Click "New integration" → give it a name (e.g., "Hack_A_Known")
  3. Set type to "Internal"
  4. Copy the **Internal Integration Token**
  5. Add it to `.env.local` as `NOTION_TOKEN=secret_xxxxx`

- [ ] **Share the database with the integration**
  1. Open the database in Notion
  2. Click `...` (top right) → `Add connections` → find your integration name
  3. Confirm access

- [ ] **Get the database ID**
  - From the Notion database URL: `https://www.notion.so/<workspace>/<DATABASE_ID>?v=<view_id>`
  - Copy the hex string between the last `/` and the `?`
  - Add it to `.env.local` as `NOTION_DATABASE_ID=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`
  - Also add both vars to Vercel environment variables in the dashboard.

- [ ] **Create `lib/notion.js`**
  - Implement `getHackathons()` using `unstable_cache` wrapping `notion.dataSources.query()`.
  - Implement `normalizeHackathon(page)` to extract clean data fields.
  - See CLAUDE.md for the exact code patterns.

- [ ] **Create `lib/dateUtils.js`**
  - Export `getStatus(startDate, endDate)` returning `'past'`, `'ongoing'`, or `'future'`.
  - See CLAUDE.md for the exact code pattern using `date-fns`.

- [ ] **Test the fetch end-to-end**
  - In `app/page.js`, call `getHackathons()` and temporarily `console.log` the result.
  - Run `npm run dev` and verify normalized hackathon objects appear in the terminal.

- [ ] **Remove the `console.log`** after confirming correct data shape.

---

## Phase 3: Table + Drawer

**Goal:** A working sortable table with a slide-over detail drawer. The Gantt tab can be a placeholder.

- [ ] **Create `components/ViewSwitcher.jsx`** (`"use client"`)
  - Wrap everything in shadcn `Tabs` with `defaultValue="table"`.
  - Two `TabsTrigger` values: `"table"` and `"gantt"`.
  - `TabsContent` for `"table"` renders `<HackathonTable>`.
  - `TabsContent` for `"gantt"` renders `<HackathonGantt>` (placeholder div is fine for now).
  - Accepts `hackathons` array as a prop.

- [ ] **Create `components/HackathonTable.jsx`** (`"use client"`)
  - Uses shadcn `Table`, `TableHeader`, `TableBody`, `TableRow`, `TableHead`, `TableCell`.
  - `useState` for `sortField` (default: `'startDate'`) and `sortDir` (default: `'asc'`).
  - Clicking a column header toggles sort on that column.
  - Columns: Name, Start Date, End Date, Tags, Status.
  - Status cell: use `getStatus()` to derive the computed status, show as a colored `Badge`.
  - Each row has an `onClick` that sets `selectedHackathon` state and `drawerOpen` to `true`.
  - Renders `<HackathonDrawer>` at the bottom of the component.

- [ ] **Create `components/HackathonDrawer.jsx`** (`"use client"`)
  - Uses shadcn `Sheet`, `SheetContent`, `SheetHeader`, `SheetTitle`.
  - Props: `hackathon` (object or null), `open` (boolean), `onClose` (function).
  - Content: Name (h2), formatted date range, Tags rendered as `Badge` list, description paragraph, "Register Now" `Button` linking to `hackathon.url` with `target="_blank"`.
  - Handles `null` hackathon gracefully (return null or render nothing when closed).

- [ ] **Update `app/page.js`**
  - Server Component (no `"use client"`).
  - Call `await getHackathons()`.
  - Return `<ViewSwitcher hackathons={hackathons} />`.

- [ ] **Wire up ISR** on the page route:
  ```js
  export const revalidate = 3600
  ```
  Add this export to `app/page.js`.

- [ ] **Verify Phase 3 end-to-end** in `npm run dev`:
  - Table renders all hackathons from Notion.
  - Clicking a column header sorts rows.
  - Clicking a row opens the drawer with correct details.
  - "Register Now" link opens the correct URL.

---

## Phase 4: Gantt View

**Goal:** A custom CSS horizontal bar timeline with color-coded status states and a "today" line.

- [ ] **Expand `lib/dateUtils.js`** with additional helpers:
  ```js
  import { min, max, subDays, addDays, differenceInDays, parseISO } from 'date-fns'

  export function computeTimelineRange(hackathons) {
    const starts = hackathons.map((h) => parseISO(h.startDate))
    const ends = hackathons.map((h) => parseISO(h.endDate))
    return {
      minDate: subDays(min(starts), 7),
      maxDate: addDays(max(ends), 7),
    }
  }

  export function toPercent(date, minDate, totalDays) {
    return (differenceInDays(date, minDate) / totalDays) * 100
  }
  ```

- [ ] **Create `components/HackathonGantt.jsx`** (`"use client"`)
  - Compute `minDate`, `maxDate`, `totalDays` from all hackathon dates using `computeTimelineRange`.
  - Render an `overflow-x-auto` outer wrapper.
  - Render a fixed-height inner timeline div with `position: relative` and a minimum width (e.g., `min-w-[900px]`).
  - For each hackathon, render a row with:
    - A label on the left (hackathon name, truncated).
    - A bar `div` with `position: absolute`, `left` and `width` set via inline styles using `toPercent()`.
    - Tailwind color classes based on `getStatus(h.startDate, h.endDate)`:
      - `past` → `bg-gray-300 text-gray-500`
      - `ongoing` → `bg-emerald-500 text-white`
      - `future` → `bg-blue-200 text-blue-800 border border-blue-400`
    - Click handler that opens `HackathonDrawer` (same as table view).
  - Render a "today" indicator: a `position: absolute`, `w-px`, `bg-red-400`, full-height vertical line at `toPercent(new Date(), minDate, totalDays)`%.
  - Render month labels along the top axis (iterate months between `minDate` and `maxDate`).

- [ ] **Replace the placeholder in `ViewSwitcher.jsx`** with `<HackathonGantt hackathons={hackathons} />`.

- [ ] **Share the drawer state** between Table and Gantt views. Lift `selectedHackathon` and `drawerOpen` state up into `ViewSwitcher.jsx` and pass `onSelect` callback down to both child components.

- [ ] **Test on mobile** — open Chrome DevTools, simulate a narrow viewport. Confirm the Gantt view scrolls horizontally and does not break layout.

- [ ] **Final build check**
  ```bash
  npm run build && npm run start
  ```
  Both table and Gantt views must render with real data. No console errors.

- [ ] **Deploy to Vercel**
  ```bash
  vercel --prod
  ```
  Or push to the `main` branch if the Vercel Git integration is connected.

- [ ] **Verify ISR on Vercel** — after the first page load, reload and check the response headers in DevTools → Network for `x-vercel-cache: HIT`. This confirms ISR is active and the CDN served the cached page.

---

## Post-Launch (Optional)

- [ ] Enable Vercel Web Analytics in the dashboard → Analytics tab and verify the `<Analytics />` component is receiving events.
- [ ] Add an on-demand revalidation Route Handler at `app/api/revalidate/route.js` protected by a secret token, so you can trigger a cache refresh immediately after editing Notion without waiting for the 1-hour window.
- [ ] Add a "Last updated" timestamp to the footer by including `_last_edited_time` from Notion page metadata.
