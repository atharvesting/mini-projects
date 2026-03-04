import { Github } from 'lucide-react';
import { theme } from '@/lib/theme';

export default function Footer({ recordsCount, lastSync, repoUrl }) {
  const year = new Date().getFullYear();

  return (
    <footer className="mt-8 border-t border-zinc-200 pt-4 dark:border-zinc-800">
      <div className="flex items-center justify-between gap-3">
        <p className={theme.subtext}>© {year} Hack-A-Know</p>

        <a
          href={repoUrl}
          target="_blank"
          rel="noopener noreferrer"
          aria-label="Open GitHub repository"
          className="inline-flex h-9 w-9 items-center justify-center rounded-full border border-zinc-300 bg-white text-zinc-800 hover:bg-zinc-100 dark:border-zinc-700 dark:bg-zinc-900 dark:text-zinc-100 dark:hover:bg-zinc-800"
        >
          <Github className="h-4 w-4" />
        </a>
      </div>

      <p className={`mt-2 ${theme.subtext}`}>
        {recordsCount} records loaded · Last successful sync: {lastSync}
      </p>
    </footer>
  );
}
