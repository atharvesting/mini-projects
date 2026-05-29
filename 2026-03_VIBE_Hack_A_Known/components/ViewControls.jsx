'use client';

import { useMemo, useState } from 'react';
import { SlidersHorizontal, X } from 'lucide-react';
import { theme } from '@/lib/theme';

function getLabel(options, value) {
  return options.find((option) => option.value === value)?.label ?? value;
}

export default function ViewControls({
  filterValue,
  onFilterChange,
  filterOptions,
  sortValue,
  onSortChange,
  sortOptions,
}) {
  const [open, setOpen] = useState(false);

  const shouldAutoCloseOnMobile = () => {
    if (typeof window === 'undefined') return false;
    return window.innerWidth < 768;
  };

  const handleFilterChange = (value) => {
    onFilterChange(value);
    if (shouldAutoCloseOnMobile()) setOpen(false);
  };

  const handleSortChange = (value) => {
    onSortChange(value);
    if (shouldAutoCloseOnMobile()) setOpen(false);
  };

  const filterLabel = useMemo(() => getLabel(filterOptions, filterValue), [filterOptions, filterValue]);
  const sortLabel = useMemo(
    () => (sortOptions?.length ? getLabel(sortOptions, sortValue) : null),
    [sortOptions, sortValue]
  );

  return (
    <div className="relative flex items-center justify-end gap-2">
      <span className={`hidden rounded-full px-2 py-1 text-xs md:inline ${theme.tagBadge}`}>{filterLabel}</span>
      {sortLabel ? (
        <span className={`hidden rounded-full px-2 py-1 text-xs md:inline ${theme.tagBadge}`}>{sortLabel}</span>
      ) : null}

      <button
        type="button"
        onClick={() => setOpen((value) => !value)}
        className="inline-flex items-center gap-2 rounded border border-zinc-200 bg-zinc-50 px-3 py-1.5 text-sm hover:bg-zinc-100 dark:border-zinc-700 dark:bg-zinc-800 dark:hover:bg-zinc-700"
      >
        {open ? <X className="h-4 w-4" /> : <SlidersHorizontal className="h-4 w-4" />}
        Controls
      </button>

      {open ? (
        <div className={`absolute right-0 top-full z-30 mt-2 min-w-[250px] rounded-lg border p-3 shadow-lg ${theme.card}`}>
          <div className="space-y-3">
            <label className="block text-sm">
              <span className={`mb-1 block ${theme.subtext}`}>Filter</span>
              <select
                value={filterValue}
                onChange={(event) => handleFilterChange(event.target.value)}
                className={`w-full ${theme.controlSelect}`}
              >
                {filterOptions.map((option) => (
                  <option key={option.value} value={option.value}>
                    {option.label}
                  </option>
                ))}
              </select>
            </label>

            {sortOptions?.length ? (
              <label className="block text-sm">
                <span className={`mb-1 block ${theme.subtext}`}>Sort</span>
                <select
                  value={sortValue}
                  onChange={(event) => handleSortChange(event.target.value)}
                  className={`w-full ${theme.controlSelect}`}
                >
                  {sortOptions.map((option) => (
                    <option key={option.value} value={option.value}>
                      {option.label}
                    </option>
                  ))}
                </select>
              </label>
            ) : null}
          </div>
        </div>
      ) : null}
    </div>
  );
}
