import {
  addDays,
  differenceInCalendarDays,
  eachMonthOfInterval,
  isAfter,
  isBefore,
  isToday,
  max,
  min,
  parseISO,
  startOfMonth,
  subDays,
} from 'date-fns';

export function parseDateSafe(value) {
  if (!value) return null;
  try {
    return parseISO(value);
  } catch {
    return null;
  }
}

export function getStatus(startDate, endDate) {
  if (!startDate || !endDate) return 'unknown';

  const today = new Date();
  const start = parseDateSafe(startDate);
  const end = parseDateSafe(endDate);

  if (!start || !end) return 'unknown';

  if (isAfter(start, today) && !isToday(start)) return 'future';
  if (isBefore(end, today) && !isToday(end)) return 'past';
  return 'ongoing';
}

export function computeTimelineRange(hackathons) {
  const starts = hackathons.map((item) => parseDateSafe(item.startDate)).filter(Boolean);
  const ends = hackathons.map((item) => parseDateSafe(item.endDate)).filter(Boolean);

  if (!starts.length || !ends.length) {
    return null;
  }

  const minDate = subDays(min(starts), 7);
  const maxDate = addDays(max(ends), 7);
  const totalDays = Math.max(1, differenceInCalendarDays(maxDate, minDate) + 1);

  return { minDate, maxDate, totalDays };
}

export function toPercent(date, minDate, totalDays) {
  const diff = differenceInCalendarDays(date, minDate);
  return (diff / totalDays) * 100;
}

export function getMonthTicks(minDate, maxDate) {
  return eachMonthOfInterval({
    start: startOfMonth(minDate),
    end: startOfMonth(maxDate),
  });
}
