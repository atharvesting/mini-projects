'use client';

import { useMemo, useState, useEffect, useRef, useLayoutEffect } from 'react';
import { format, addMonths, subMonths, differenceInCalendarDays } from 'date-fns';
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
  const [todayHoverY, setTodayHoverY] = useState(null);
  const [zoom, setZoom] = useState(1);

  const containerRef = useRef(null);
  const lastZoomFocusRef = useRef(null);
  const hasInitialized = useRef(false);

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

  useEffect(() => {
    const container = containerRef.current;
    if (!container) return;

    const handleWheel = (e) => {
      if (e.ctrlKey || e.metaKey) {
        e.preventDefault();
        const zoomOut = e.deltaY > 0;
        const zoomFactor = 0.15;

        setZoom((prevZoom) => {
          const newZoom = prevZoom * (zoomOut ? 1 - zoomFactor : 1 + zoomFactor);
          const clampedZoom = Math.max(0.5, Math.min(newZoom, 20));

          lastZoomFocusRef.current = {
            mouseX: e.clientX,
            containerRect: container.getBoundingClientRect(),
            scrollLeft: container.scrollLeft,
            oldZoom: prevZoom,
            newZoom: clampedZoom,
          };

          return clampedZoom;
        });
      }
    };

    container.addEventListener('wheel', handleWheel, { passive: false });
    return () => container.removeEventListener('wheel', handleWheel);
  }, []);

  useLayoutEffect(() => {
    if (lastZoomFocusRef.current && containerRef.current) {
      const focus = lastZoomFocusRef.current;
      const container = containerRef.current;

      const rect = focus.containerRect;
      const cursorRelativeX = focus.mouseX - rect.left;
      const contentX = focus.scrollLeft + cursorRelativeX;

      const scale = focus.newZoom / focus.oldZoom;

      let scaledContentX;
      if (contentX > 236) {
        scaledContentX = 236 + (contentX - 236) * scale;
      } else {
        scaledContentX = contentX;
      }

      container.scrollLeft = scaledContentX - cursorRelativeX;
      lastZoomFocusRef.current = null;
    }
  }, [zoom]);

  const resetToToday = () => {
    setZoom(1);
    requestAnimationFrame(() => {
      if (!containerRef.current || !timeline) return;
      const trackWidth = containerRef.current.scrollWidth - 236;
      const centerOffset = 236 + (todayPercent / 100) * trackWidth;
      containerRef.current.scrollLeft = centerOffset - containerRef.current.clientWidth / 2;
    });
  };

  return (
    <div className="space-y-4">
      <div className="flex flex-col sm:flex-row items-end sm:items-center justify-between gap-3">
        <button
          onClick={resetToToday}
          className="inline-flex shrink-0 items-center justify-center gap-2 rounded border border-zinc-200 bg-white px-3 py-1.5 text-sm font-medium hover:bg-zinc-50 dark:border-zinc-800 dark:bg-zinc-950 dark:hover:bg-zinc-900"
        >
          Go to Today
        </button>
        <div className="w-full sm:w-auto">
          <ViewControls
            filterValue={filter}
            onFilterChange={setFilter}
            filterOptions={filterOptions}
            sortValue={sortMode}
            onSortChange={setSortMode}
            sortOptions={sortOptions}
          />
        </div>
      </div>

      <div className={`${theme.card} p-4 md:p-6`}>

        <div className="overflow-x-auto pb-4" ref={containerRef}>
        <div className="min-w-[900px]" style={{ width: `${Math.max(100, zoom * 100)}%` }}>
          <div className="grid grid-cols-[220px_1fr] gap-4 border-b border-zinc-200 pb-3 dark:border-zinc-800 relative">
            <div className={`sticky left-0 z-20 bg-white dark:bg-zinc-900 text-xs font-medium uppercase tracking-wide ${theme.subtext}`}>Hackathon</div>
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
              <div className="pointer-events-none absolute inset-0 z-10 grid grid-cols-[220px_1fr] gap-4">
                <div />
                <div className="relative h-full">
                  <div
                    className="pointer-events-auto absolute top-0 bottom-0 flex w-4 -translate-x-1/2 justify-center group cursor-default"
                    style={{ left: `${todayPercent}%` }}
                    onMouseMove={(e) => {
                      const rect = e.currentTarget.getBoundingClientRect();
                      setTodayHoverY(e.clientY - rect.top);
                    }}
                    onMouseLeave={() => setTodayHoverY(null)}
                  >
                    <div className="h-full w-[1px] bg-red-400 transition-all duration-200 group-hover:w-[3px]" />
                    <div
                      className={`absolute z-50 w-max flex-col items-center rounded-md bg-zinc-900 px-2 py-1 text-xs text-white shadow-md dark:bg-zinc-100 dark:text-zinc-900 ${
                        todayHoverY !== null ? 'flex' : 'hidden'
                      }`}
                      style={{ top: todayHoverY !== null ? todayHoverY : 8, left: '100%', marginLeft: '8px' }}
                    >
                      <span className="font-semibold">Today</span>
                      <span className="text-[10px] opacity-80">{format(today, 'MMM d, yyyy')}</span>
                    </div>
                  </div>
                </div>
              </div>
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
                  <div key={hackathon.id} className="grid grid-cols-[220px_1fr] items-center gap-4 relative">
                    <div className="sticky left-0 z-20 flex h-full items-center bg-white pr-2 dark:bg-zinc-900">
                      <button
                        onClick={() => onSelect(hackathon)}
                        className="truncate text-left text-sm font-medium hover:underline w-full"
                        title={hackathon.name}
                      >
                        {hackathon.name}
                      </button>
                    </div>

                    <div className="relative h-9 overflow-hidden rounded bg-zinc-100/70 dark:bg-zinc-800/60 w-full">
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