'use client';

import { useEffect } from 'react';
import { format, parseISO } from 'date-fns';
import { X } from 'lucide-react';
import { Badge } from '@/components/ui/badge';
import { Button } from '@/components/ui/button';
import { theme } from '@/lib/theme';

function formatDate(value) {
  if (!value) return 'TBD';
  try {
    return format(parseISO(value), 'MMM d, yyyy');
  } catch {
    return value;
  }
}

export default function HackathonDrawer({ hackathon, open, onOpenChange }) {
  useEffect(() => {
    if (!open) return;

    const onKeyDown = (event) => {
      if (event.key === 'Escape') onOpenChange(false);
    };

    window.addEventListener('keydown', onKeyDown);
    return () => window.removeEventListener('keydown', onKeyDown);
  }, [open, onOpenChange]);

  if (!open || !hackathon) return null;

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center p-4">
      <button
        aria-label="Close details"
        className="absolute inset-0 bg-black/45"
        onClick={() => onOpenChange(false)}
      />

      <div className="relative z-10 w-full max-w-2xl overflow-hidden rounded-lg border border-zinc-200 bg-white shadow-lg dark:border-zinc-800 dark:bg-zinc-900">
        <div className="flex items-start justify-between border-b border-zinc-200 px-5 py-4 dark:border-zinc-800">
          <div>
            <h2 className="text-xl font-semibold">{hackathon.name}</h2>
            <p className={theme.subtext}>
              {formatDate(hackathon.startDate)} - {formatDate(hackathon.endDate)}
            </p>
          </div>

          <button
            className="rounded p-1 text-zinc-500 hover:bg-zinc-100 hover:text-zinc-900 dark:hover:bg-zinc-800 dark:hover:text-zinc-100"
            onClick={() => onOpenChange(false)}
            aria-label="Close"
          >
            <X className="h-5 w-5" />
          </button>
        </div>

        <div className="max-h-[70vh] space-y-6 overflow-y-auto px-5 py-4">
          <div>
            <p className={`mb-2 text-sm font-medium ${theme.subtext}`}>Tags</p>
            <div className="flex flex-wrap gap-2">
              {hackathon.tags?.length ? (
                hackathon.tags.map((tag) => (
                  <Badge key={tag} className={theme.tagBadge}>
                    {tag}
                  </Badge>
                ))
              ) : (
                <span className={theme.subtext}>No tags</span>
              )}
            </div>
          </div>

          <div>
            <p className={`mb-2 text-sm font-medium ${theme.subtext}`}>Description</p>
            <p className="text-sm leading-6 whitespace-pre-wrap">
              {hackathon.description || 'No description provided.'}
            </p>
          </div>

          {hackathon.url ? (
            <Button asChild className="w-full">
              <a href={hackathon.url} target="_blank" rel="noopener noreferrer">
                Open Registration
              </a>
            </Button>
          ) : null}
        </div>
      </div>
    </div>
  );
}
