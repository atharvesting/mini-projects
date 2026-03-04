'use client';

import { useMemo, useState } from 'react';
import { format } from 'date-fns';
import {
  computeTimelineRange,
  getMonthTicks,
  getStatus,
  parseDateSafe,
  toPercent,
} from '@/lib/dateUtils';
import { theme } from '@/lib/theme';
import ViewControls from '@/components/ViewControls';

export default function HackathonGantt({ hackathons, onSelect }) {
  const [sortMode, setSortMode] = useState('start-asc');
  const [filter, setFilter] = useState('active');
  const [hoverInfo, setHoverInfo] = useState(null);

  const filterOptions = [
    { value: 'active', label: 'Active (ongoing + future)' },
    { value: 'all', label: 'All events' },
    { value: 'ongoing', label: 'Ongoing only' },
    { value: 'future', label: 'Future only' },
    { value: 'past', label: 'Past only' },
  ];

  const sortOptions = [
    { value: 'start-asc', label: 'Earliest start first' },
    { value: 'start-desc', label: 'Latest start first' },
    { value: 'end-asc', label: 'Earliest deadline first' },
    { value: 'end-desc', label: 'Latest deadline first' },
    { value: 'name-asc', label: 'Name A-Z' },
    { value: 'name-desc', label: 'Name Z-A' },
  ];

  const items = useMemo(() => {
    const valid = hackathons
      .filter((item) => parseDateSafe(item.startDate) && parseDateSafe(item.endDate))
      .filter((item) => {
        const status = getStatus(item.startDate, item.endDate);

        if (filter === 'all') return true;
        if (filter === 'active') return status === 'ongoing' || status === 'future';
        if (filter === 'ongoing') return status === 'ongoing';
        if (filter === 'future') return status === 'future';
        if (filter === 'past') return status === 'past';
        return true;
      });

    return valid.sort((a, b) => {
      const startA = parseDateSafe(a.startDate)?.getTime() ?? 0;
      const startB = parseDateSafe(b.startDate)?.getTime() ?? 0;
      const endA = parseDateSafe(a.endDate)?.getTime() ?? 0;
      const endB = parseDateSafe(b.endDate)?.getTime() ?? 0;

      if (sortMode === 'start-asc') return startA - startB;
      if (sortMode === 'start-desc') return startB - startA;
      if (sortMode === 'end-asc') return endA - endB;
      if (sortMode === 'end-desc') return endB - endA;
      if (sortMode === 'name-asc') return (a.name ?? '').localeCompare(b.name ?? '');
      if (sortMode === 'name-desc') return (b.name ?? '').localeCompare(a.name ?? '');
      return 0;
    });
  }, [hackathons, filter, sortMode]);

  const timeline = computeTimelineRange(items);

  if (!timeline) {
    return (
      <div className={`${theme.card} p-4 md:p-6`}>
        <p className={`mt-2 ${theme.subtext}`}>
          Add valid start and end dates in Notion to render the timeline.
        </p>
      </div>
    );
  }

  const { minDate, maxDate, totalDays } = timeline;
  const monthTicks = getMonthTicks(minDate, maxDate);
  const today = new Date();
  const todayPercent = toPercent(today, minDate, totalDays);
  const showTodayLine = todayPercent >= 0 && todayPercent <= 100;

  return (
    <div className="space-y-2">
      <ViewControls
        filterValue={filter}
        onFilterChange={setFilter}
        filterOptions={filterOptions}
        sortValue={sortMode}
        onSortChange={setSortMode}
        sortOptions={sortOptions}
      />

      <div className={`${theme.card} p-4 md:p-6`}>

        <div className="mt-4 overflow-x-auto">
        <div className="min-w-[900px]">
          <div className="grid grid-cols-[220px_1fr] gap-4 border-b border-zinc-200 pb-3 dark:border-zinc-800">
            <div className={`text-xs font-medium uppercase tracking-wide ${theme.subtext}`}>Hackathon</div>
            <div className="relative h-8">
              {monthTicks.map((month) => {
                const left = toPercent(month, minDate, totalDays);
                return (
                  <div
                    key={month.toISOString()}
                    className="absolute top-0 -translate-x-1/2 text-xs text-zinc-500 dark:text-zinc-400"
                    style={{ left: `${left}%` }}
                  >
                    {format(month, 'MMM yyyy')}
                  </div>
                );
              })}
            </div>
          </div>

          <div className="relative mt-2">
            {showTodayLine ? (
              <div
                className="pointer-events-none absolute top-0 bottom-0 z-10 w-px bg-red-400"
                style={{ left: `calc(220px + 1rem + ${todayPercent}%)` }}
              />
            ) : null}

            <div className="space-y-2">
              {items.map((hackathon) => {
                const start = parseDateSafe(hackathon.startDate);
                const end = parseDateSafe(hackathon.endDate);
                const status = getStatus(hackathon.startDate, hackathon.endDate);

                const left = toPercent(start, minDate, totalDays);
                const barWidth = Math.max(
                  (Math.max(1, (end - start) / (1000 * 60 * 60 * 24) + 1) / totalDays) * 100,
                  0.8
                );
                const hoverStart = format(start, 'MMM d, yyyy');
                const hoverEnd = format(end, 'MMM d, yyyy');

                return (
                  <div key={hackathon.id} className="grid grid-cols-[220px_1fr] items-center gap-4">
                    <button
                      onClick={() => onSelect(hackathon)}
                      className="truncate text-left text-sm font-medium hover:underline"
                      title={hackathon.name}
                    >
                      {hackathon.name}
                    </button>

                    <div className="relative h-9 rounded bg-zinc-100/70 dark:bg-zinc-800/60">
                      <button
                        onClick={() => onSelect(hackathon)}
                        onMouseEnter={(event) =>
                          setHoverInfo({
                            x: event.clientX,
                            y: event.clientY,
                            name: hackathon.name,
                            status,
                            start: hoverStart,
                            end: hoverEnd,
                          })
                        }
                        onMouseMove={(event) =>
                          setHoverInfo((prev) =>
                            prev
                              ? {
                                  ...prev,
                                  x: event.clientX,
                                  y: event.clientY,
                                }
                              : prev
                          )
                        }
                        onMouseLeave={() => setHoverInfo(null)}
                        className={`absolute top-1/2 h-7 -translate-y-1/2 rounded px-2 text-left text-xs font-medium ${theme.status[status] || theme.status.unknown}`}
                        style={{ left: `${left}%`, width: `${barWidth}%` }}
                      >
                        <span className="block truncate">{hackathon.name}</span>
                      </button>
                    </div>
                  </div>
                );
              })}
            </div>
          </div>
        </div>
      </div>

      {hoverInfo ? (
        <div
          className={theme.tooltip}
          style={{ left: hoverInfo.x + 12, top: hoverInfo.y + 12 }}
        >
          <p className="font-semibold">{hoverInfo.name}</p>
          <p className={theme.subtext}>Status: {hoverInfo.status}</p>
          <p>{hoverInfo.start} → {hoverInfo.end}</p>
        </div>
      ) : null}
      </div>
    </div>
  );
}
