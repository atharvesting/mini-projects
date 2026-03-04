'use client';

import { useEffect, useState } from 'react';

export default function RefreshShortcut() {
  const [lastTriggeredAt, setLastTriggeredAt] = useState(0);

  useEffect(() => {
    const onKeyDown = (event) => {
      const isShortcut = event.ctrlKey && event.shiftKey && event.key.toLowerCase() === 'u';
      if (!isShortcut) return;

      event.preventDefault();

      const now = Date.now();
      if (now - lastTriggeredAt < 2000) return;

      setLastTriggeredAt(now);
      window.location.reload();
    };

    window.addEventListener('keydown', onKeyDown);
    return () => window.removeEventListener('keydown', onKeyDown);
  }, [lastTriggeredAt]);

  return null;
}
