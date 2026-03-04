'use client';

import { useState } from 'react';
import HackathonDrawer from '@/components/HackathonDrawer';
import HackathonGantt from '@/components/HackathonGantt';
import RefreshShortcut from '@/components/RefreshShortcut';
import HackathonTable from '@/components/HackathonTable';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';

export default function ViewSwitcher({ hackathons }) {
  const [selectedHackathon, setSelectedHackathon] = useState(null);
  const [drawerOpen, setDrawerOpen] = useState(false);

  const handleSelect = (hackathon) => {
    setSelectedHackathon(hackathon);
    setDrawerOpen(true);
  };

  return (
    <>
      <RefreshShortcut />
      <Tabs defaultValue="table" className="w-full">
        <TabsList className="mb-4 mx-auto">
          <TabsTrigger value="table">Table View</TabsTrigger>
          <TabsTrigger value="gantt">Timeline View</TabsTrigger>
        </TabsList>

        <TabsContent value="table">
          <HackathonTable hackathons={hackathons} onSelect={handleSelect} />
        </TabsContent>

        <TabsContent value="gantt">
          <HackathonGantt hackathons={hackathons} onSelect={handleSelect} />
        </TabsContent>
      </Tabs>

      <HackathonDrawer
        hackathon={selectedHackathon}
        open={drawerOpen}
        onOpenChange={setDrawerOpen}
      />
    </>
  );
}
