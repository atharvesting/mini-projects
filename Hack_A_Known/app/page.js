import { getHackathonsData } from '@/lib/notion';
import Footer from '@/components/Footer';
import ThemeToggle from '@/components/ThemeToggle';
import ViewSwitcher from '@/components/ViewSwitcher';
import { siteConfig } from '@/lib/siteConfig';
import { theme } from '@/lib/theme';

export const revalidate = 120;

function formatLastSync(value) {
  if (!value) return 'Unknown';

  const date = new Date(value);
  if (Number.isNaN(date.getTime())) return 'Unknown';

  return new Intl.DateTimeFormat('en-US', {
    dateStyle: 'medium',
    timeStyle: 'short',
  }).format(date);
}

export default async function Home() {
  const data = await getHackathonsData();
  const hackathons = data.items;
  const lastSync = formatLastSync(data.fetchedAt);

  return (
    <div className={`${theme.page} flex min-h-screen flex-col`}>
      <main className={`${theme.container} flex flex-1 flex-col`}>
        <div className="mb-2 flex items-center justify-center gap-4">
          <h1 className="text-center text-3xl font-semibold md:text-4xl">{siteConfig.name}</h1>
          <ThemeToggle />
        </div>
        <div className="mb-6" />

        <div className="flex-1">
          <ViewSwitcher hackathons={hackathons} />
        </div>

        <Footer recordsCount={hackathons.length} lastSync={lastSync} repoUrl={siteConfig.repoUrl} />
      </main>
    </div>
  );
}
