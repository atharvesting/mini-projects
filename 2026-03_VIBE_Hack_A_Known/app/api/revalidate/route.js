import { revalidatePath, revalidateTag } from 'next/cache';
import { NextResponse } from 'next/server';

export async function POST(request) {
  const secret = process.env.REVALIDATE_SECRET;

  if (!secret) {
    return NextResponse.json(
      { ok: false, message: 'Missing REVALIDATE_SECRET on server.' },
      { status: 500 }
    );
  }

  const authHeader = request.headers.get('authorization');
  const bearer = authHeader?.startsWith('Bearer ') ? authHeader.slice(7) : null;
  const providedSecret = bearer || new URL(request.url).searchParams.get('secret');

  if (providedSecret !== secret) {
    return NextResponse.json({ ok: false, message: 'Unauthorized.' }, { status: 401 });
  }

  revalidateTag('hackathons');
  revalidatePath('/');

  return NextResponse.json({
    ok: true,
    revalidated: true,
    tag: 'hackathons',
    path: '/',
    at: new Date().toISOString(),
  });
}
