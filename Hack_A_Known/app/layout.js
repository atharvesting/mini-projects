import { Geist, Geist_Mono } from "next/font/google";
import { Analytics } from "@vercel/analytics/react";
import "./globals.css";

const themeBootstrapScript = `(() => {
  try {
    const savedTheme = window.localStorage.getItem('hack-a-know-theme');
    const isDark = savedTheme === 'dark';
    document.documentElement.classList.toggle('dark', isDark);
  } catch {
    document.documentElement.classList.remove('dark');
  }
})();`;

const geistSans = Geist({
  variable: "--font-geist-sans",
  subsets: ["latin"],
});

const geistMono = Geist_Mono({
  variable: "--font-geist-mono",
  subsets: ["latin"],
});

export const metadata = {
  title: "Hack-A-Know",
  description: "Curated hackathons in table and timeline views.",
};

export default function RootLayout({ children }) {
  return (
    <html lang="en" suppressHydrationWarning>
      <head>
        <script dangerouslySetInnerHTML={{ __html: themeBootstrapScript }} />
      </head>
      <body
        className={`${geistSans.variable} ${geistMono.variable} antialiased`}
      >
        {children}
        <Analytics />
      </body>
    </html>
  );
}
