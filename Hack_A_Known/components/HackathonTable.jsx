'use client';

import { useMemo, useState } from 'react';
import { format, parseISO } from 'date-fns';
import { ArrowDown, ArrowUp, ArrowUpDown } from 'lucide-react';
import { Badge } from '@/components/ui/badge';
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from '@/components/ui/table';
import { getStatus } from '@/lib/dateUtils';
import { theme } from '@/lib/theme';
import ViewControls from '@/components/ViewControls';

function formatDate(value) {
  if (!value) return 'TBD';
  try {
    return format(parseISO(value), 'MMM d, yyyy');
  } catch {
    return value;
  }
}

function getSortValue(item, field) {
  if (field === 'name') return item.name?.toLowerCase() ?? '';
  if (field === 'startDate') return item.startDate ?? '';
  if (field === 'status') return getStatus(item.startDate, item.endDate);
  return '';
}

function SortIcon({ active, direction }) {
  if (!active) return <ArrowUpDown className="h-4 w-4" />;
  return direction === 'asc' ? <ArrowUp className="h-4 w-4" /> : <ArrowDown className="h-4 w-4" />;
}

function formatStatusLabel(value) {
  if (!value) return 'Unknown';
  return value.charAt(0).toUpperCase() + value.slice(1);
}

export default function HackathonTable({ hackathons, onSelect }) {
  const [sortField, setSortField] = useState('startDate');
  const [sortDirection, setSortDirection] = useState('asc');
  const [filter, setFilter] = useState('active');

  const filterOptions = [
    { value: 'active', label: 'Active (ongoing + future)' },
    { value: 'all', label: 'All events' },
    { value: 'ongoing', label: 'Ongoing only' },
    { value: 'future', label: 'Future only' },
    { value: 'past', label: 'Past only' },
  ];

  const sortOptions = [
    { value: 'name-asc', label: 'Name A-Z' },
    { value: 'name-desc', label: 'Name Z-A' },
    { value: 'startDate-asc', label: 'Earliest start first' },
    { value: 'startDate-desc', label: 'Latest start first' },
    { value: 'status-asc', label: 'Status A-Z' },
    { value: 'status-desc', label: 'Status Z-A' },
  ];

  const sortMode = `${sortField}-${sortDirection}`;

  const handleSortModeChange = (value) => {
    const [field, direction] = value.split('-');
    setSortField(field);
    setSortDirection(direction);
  };

  const filteredHackathons = useMemo(() => {
    return hackathons.filter((item) => {
      const status = getStatus(item.startDate, item.endDate);

      if (filter === 'all') return true;
      if (filter === 'active') return status === 'ongoing' || status === 'future';
      if (filter === 'ongoing') return status === 'ongoing';
      if (filter === 'future') return status === 'future';
      if (filter === 'past') return status === 'past';
      return true;
    });
  }, [hackathons, filter]);

  const sortedHackathons = useMemo(() => {
    const clone = [...filteredHackathons];
    clone.sort((a, b) => {
      const aValue = getSortValue(a, sortField);
      const bValue = getSortValue(b, sortField);
      if (aValue < bValue) return sortDirection === 'asc' ? -1 : 1;
      if (aValue > bValue) return sortDirection === 'asc' ? 1 : -1;
      return 0;
    });
    return clone;
  }, [filteredHackathons, sortField, sortDirection]);

  const handleSort = (field) => {
    if (field === sortField) {
      setSortDirection((prev) => (prev === 'asc' ? 'desc' : 'asc'));
      return;
    }

    setSortField(field);
    setSortDirection('asc');
  };

  return (
    <div className="space-y-2">
      <ViewControls
        filterValue={filter}
        onFilterChange={setFilter}
        filterOptions={filterOptions}
        sortValue={sortMode}
        onSortChange={handleSortModeChange}
        sortOptions={sortOptions}
      />

      <div className={`${theme.card} overflow-hidden`}>
        <Table>
          <TableHeader>
            <TableRow>
              <TableHead>
                <button className={theme.tableHeaderButton} onClick={() => handleSort('name')}>
                  Name
                  <SortIcon active={sortField === 'name'} direction={sortDirection} />
                </button>
              </TableHead>
              <TableHead>
                <button className={theme.tableHeaderButton} onClick={() => handleSort('startDate')}>
                  Start Date
                  <SortIcon active={sortField === 'startDate'} direction={sortDirection} />
                </button>
              </TableHead>
              <TableHead>End Date</TableHead>
              <TableHead>Tags</TableHead>
              <TableHead>
                <button className={theme.tableHeaderButton} onClick={() => handleSort('status')}>
                  Status
                  <SortIcon active={sortField === 'status'} direction={sortDirection} />
                </button>
              </TableHead>
            </TableRow>
          </TableHeader>
          <TableBody>
            {sortedHackathons.length ? (
              sortedHackathons.map((hackathon) => {
                const status = getStatus(hackathon.startDate, hackathon.endDate);
                return (
                  <TableRow
                    key={hackathon.id}
                    className={theme.tableRow}
                    onClick={() => onSelect(hackathon)}
                  >
                    <TableCell className="font-medium">{hackathon.name}</TableCell>
                    <TableCell>{formatDate(hackathon.startDate)}</TableCell>
                    <TableCell>{formatDate(hackathon.endDate)}</TableCell>
                    <TableCell>
                      <div className="flex flex-wrap gap-1">
                        {hackathon.tags?.slice(0, 3).map((tag) => (
                          <Badge key={tag} className={theme.tagBadge}>
                            {tag}
                          </Badge>
                        ))}
                      </div>
                    </TableCell>
                    <TableCell>
                      <Badge className={theme.status[status] || theme.status.unknown}>
                        {formatStatusLabel(status)}
                      </Badge>
                    </TableCell>
                  </TableRow>
                );
              })
            ) : (
              <TableRow>
                <TableCell colSpan={5} className={`py-10 text-center ${theme.subtext}`}>
                  No hackathons found.
                </TableCell>
              </TableRow>
            )}
          </TableBody>
        </Table>
      </div>
    </div>
  );
}
