'use client';

import { useEffect, useState } from 'react';

const THEME_KEY = 'hack-a-know-theme';

function resolveInitialTheme() {
  if (typeof window === 'undefined') return 'light';

  const savedTheme = window.localStorage.getItem(THEME_KEY);
  if (savedTheme === 'light' || savedTheme === 'dark') return savedTheme;

  return 'light';
}

export default function ThemeToggle() {
  const [theme, setTheme] = useState('light');

  useEffect(() => {
    const currentTheme = resolveInitialTheme();
    setTheme(currentTheme);
    document.documentElement.classList.toggle('dark', currentTheme === 'dark');
  }, []);

  const toggleTheme = () => {
    const nextTheme = theme === 'dark' ? 'light' : 'dark';
    setTheme(nextTheme);
    document.documentElement.classList.toggle('dark', nextTheme === 'dark');
    window.localStorage.setItem(THEME_KEY, nextTheme);
  };

  const isDark = theme === 'dark';

  return (
    <button
      type="button"
      aria-label={isDark ? 'Switch to light mode' : 'Switch to dark mode'}
      onClick={toggleTheme}
      className="inline-flex h-8 w-8 items-center justify-center rounded-full border-[3px] border-zinc-900 bg-zinc-50 transition-colors hover:bg-zinc-100 dark:border-zinc-100 dark:bg-zinc-900 dark:hover:bg-zinc-800"
    />
  );
}
