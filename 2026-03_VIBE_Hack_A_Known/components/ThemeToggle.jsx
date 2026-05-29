'use client';

import { useEffect } from 'react';

const THEME_KEY = 'hack-a-know-theme';

function resolveInitialTheme() {
  if (typeof window === 'undefined') return 'light';

  const savedTheme = window.localStorage.getItem(THEME_KEY);
  if (savedTheme === 'light' || savedTheme === 'dark') return savedTheme;

  return 'light';
}

export default function ThemeToggle() {
  useEffect(() => {
    const currentTheme = resolveInitialTheme();
    document.documentElement.classList.toggle('dark', currentTheme === 'dark');
  }, []);

  const toggleTheme = () => {
    const nextIsDark = !document.documentElement.classList.contains('dark');
    document.documentElement.classList.toggle('dark', nextIsDark);
    window.localStorage.setItem(THEME_KEY, nextIsDark ? 'dark' : 'light');
  };

  return (
    <button
      type="button"
      aria-label="Toggle color theme"
      onClick={toggleTheme}
      className="inline-flex h-8 w-8 items-center justify-center rounded-full border-[3px] border-zinc-900 bg-zinc-50 transition-colors hover:bg-zinc-100 dark:border-zinc-100 dark:bg-zinc-900 dark:hover:bg-zinc-800"
    />
  );
}
