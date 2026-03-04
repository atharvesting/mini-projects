export const theme = {
  page: 'min-h-screen bg-zinc-50 text-zinc-900 dark:bg-zinc-950 dark:text-zinc-50',
  container: 'mx-auto w-full max-w-6xl p-6 md:p-8',
  card: 'rounded-lg border border-zinc-200 bg-white dark:border-zinc-800 dark:bg-zinc-900',
  subtext: 'text-sm text-zinc-600 dark:text-zinc-400',
  tableHeaderButton:
    'inline-flex items-center gap-1 font-medium text-zinc-700 hover:text-zinc-900 dark:text-zinc-300 dark:hover:text-zinc-100',
  controlSelect:
    'rounded border border-zinc-200 bg-zinc-50 px-3 py-1 text-sm text-zinc-800 outline-none hover:bg-zinc-100 focus:ring-2 focus:ring-zinc-300 dark:border-zinc-700 dark:bg-zinc-800 dark:text-zinc-100 dark:hover:bg-zinc-700 dark:focus:ring-zinc-600',
  controlButton:
    'inline-flex items-center gap-2 rounded border border-zinc-200 bg-zinc-50 px-3 py-1.5 text-sm hover:bg-zinc-100 dark:border-zinc-700 dark:bg-zinc-800 dark:hover:bg-zinc-700',
  tableRow: 'cursor-pointer',
  tagBadge: 'bg-zinc-100 text-zinc-700 dark:bg-zinc-800 dark:text-zinc-200',
  tooltip:
    'pointer-events-none fixed z-[60] rounded border border-zinc-200 bg-white px-3 py-2 text-xs shadow-lg dark:border-zinc-700 dark:bg-zinc-900',
  status: {
    past: 'bg-zinc-200 text-zinc-700 dark:bg-zinc-800 dark:text-zinc-300',
    ongoing: 'bg-emerald-500 text-white',
    future: 'border border-blue-300 bg-blue-100 text-blue-800 dark:border-blue-700 dark:bg-blue-900/40 dark:text-blue-200',
    unknown: 'bg-amber-100 text-amber-800 dark:bg-amber-900/40 dark:text-amber-200',
  },
};
