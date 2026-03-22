'use client';

import { useRef, useState } from 'react';
import { CalendarDays, Plus, X } from 'lucide-react';
import { theme } from '@/lib/theme';
import { suggestHackathon } from '@/app/actions';

export default function SuggestionModal() {
  const [open, setOpen] = useState(false);
  const [loading, setLoading] = useState(false);
  const [status, setStatus] = useState(null); // null, 'success', 'error'
  const [errorMessage, setErrorMessage] = useState('');
  const [startDate, setStartDate] = useState('');
  const [endDate, setEndDate] = useState('');
  const startDateInputRef = useRef(null);
  const endDateInputRef = useRef(null);

  const formatIsoToDisplay = (isoDate) => {
    if (!isoDate) return '';
    const parts = isoDate.split('-');
    if (parts.length !== 3) return '';
    return `${parts[2]}/${parts[1]}/${parts[0]}`;
  };

  const openDatePicker = (inputRef) => {
    const input = inputRef.current;
    if (!input) return;

    if (typeof input.showPicker === 'function') {
      input.showPicker();
      return;
    }

    input.focus();
    input.click();
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setStatus(null);
    setErrorMessage('');

    const today = new Date();
    today.setHours(0, 0, 0, 0);

    if (!endDate) {
      setStatus('error');
      setErrorMessage('End date is required for suggestions.');
      return;
    }

    const parsedEndDate = new Date(`${endDate}T00:00:00`);
    if (Number.isNaN(parsedEndDate.getTime())) {
      setStatus('error');
      setErrorMessage('Please select a valid end date.');
      return;
    }

    if (parsedEndDate < today) {
      setStatus('error');
      setErrorMessage('Suggestions for hackathons conducted in the past are not allowed.');
      return;
    }

    setLoading(true);

    const formData = new FormData(e.currentTarget);
    const result = await suggestHackathon(formData);

    setLoading(false);
    if (result?.error) {
      setStatus('error');
      setErrorMessage(result.error);
    } else {
      setStatus('success');
      setTimeout(() => {
        setOpen(false);
        setStatus(null);
      }, 2000);
    }
  };

  return (
    <>
      <button
        onClick={() => setOpen(true)}
        className="inline-flex items-center gap-2 rounded bg-zinc-900 px-3 py-1.5 text-sm font-medium text-white transition-colors hover:bg-zinc-800 dark:bg-zinc-100 dark:text-zinc-900 dark:hover:bg-zinc-200 w-full md:w-auto justify-center"
      >
        <Plus className="h-4 w-4" />
        Suggest Event
      </button>

      {open && (
        <div className="fixed inset-0 z-[100] flex items-center justify-center bg-black/40 p-4 backdrop-blur-sm">
          <button
            aria-label="Close"
            className="absolute inset-0 cursor-default"
            onClick={() => !loading && setOpen(false)}
          />
          
          <div className="relative z-[101] w-full max-w-sm overflow-hidden rounded-xl border border-zinc-200 bg-white p-5 shadow-2xl dark:border-zinc-800 dark:bg-zinc-900">
            <div className="mb-4 flex items-center justify-between">
              <h2 className="text-lg font-semibold text-zinc-900 dark:text-zinc-100">Suggest an Event</h2>
              <button
                onClick={() => setOpen(false)}
                className="rounded p-1 text-zinc-500 hover:bg-zinc-100 hover:text-zinc-900 dark:hover:bg-zinc-800 dark:hover:text-zinc-100 transition-colors"
                disabled={loading}
              >
                <X className="h-5 w-5" />
              </button>
            </div>

            {status === 'success' ? (
              <div className="flex h-32 flex-col items-center justify-center text-center">
                <div className="mb-3 rounded-full bg-emerald-100 p-2 text-emerald-600 dark:bg-emerald-900/30 dark:text-emerald-400">
                  <svg className="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M5 13l4 4L19 7" />
                  </svg>
                </div>
                <p className="font-medium text-zinc-900 dark:text-zinc-100">Suggestion submitted!</p>
                <p className={theme.subtext}>It will appear once approved.</p>
              </div>
            ) : (
              <form onSubmit={handleSubmit} className="space-y-4 overflow-y-auto max-h-[70vh] p-1 -mx-1">
                <div>
                  <label htmlFor="name" className={`mb-1 block text-sm font-medium ${theme.subtext}`}>
                    Hackathon Name <span className="text-red-500">*</span>
                  </label>
                  <input
                    id="name"
                    name="name"
                    type="text"
                    required
                    placeholder="e.g. Smart India Hackathon"
                    className={`w-full ${theme.controlSelect} text-base`}
                  />
                </div>

                <div>
                  <label htmlFor="url" className={`mb-1 block text-sm font-medium ${theme.subtext}`}>
                    Website / Link <span className="text-red-500">*</span>
                  </label>
                  <input
                    id="url"
                    name="url"
                    type="url"
                    required
                    placeholder="https://"
                    className={`w-full ${theme.controlSelect} text-base`}
                  />
                </div>

                <div className="grid grid-cols-2 gap-3">
                  <div>
                    <label htmlFor="startDate" className={`mb-1 block text-sm font-medium ${theme.subtext}`}>
                      Start Date
                    </label>
                    <div className="relative">
                      <input
                        type="text"
                        value={formatIsoToDisplay(startDate)}
                        placeholder="DD/MM/YYYY"
                        readOnly
                        onClick={() => openDatePicker(startDateInputRef)}
                        onKeyDown={(e) => {
                          if (e.key === 'Enter' || e.key === ' ') {
                            e.preventDefault();
                            openDatePicker(startDateInputRef);
                          }
                        }}
                        className={`w-full pr-9 ${theme.controlSelect} text-base cursor-pointer`}
                      />
                      <CalendarDays className="pointer-events-none absolute right-2.5 top-1/2 h-4 w-4 -translate-y-1/2 text-zinc-500 dark:text-zinc-400" />
                      <input
                        ref={startDateInputRef}
                        id="startDate"
                        name="startDate"
                        type="date"
                        value={startDate}
                        onChange={(e) => setStartDate(e.target.value)}
                        className="absolute h-0 w-0 opacity-0"
                        tabIndex={-1}
                        aria-hidden="true"
                      />
                    </div>
                  </div>
                  <div>
                    <label htmlFor="endDate" className={`mb-1 block text-sm font-medium ${theme.subtext}`}>
                      End Date <span className="text-red-500">*</span>
                    </label>
                    <div className="relative">
                      <input
                        type="text"
                        value={formatIsoToDisplay(endDate)}
                        placeholder="DD/MM/YYYY"
                        readOnly
                        onClick={() => openDatePicker(endDateInputRef)}
                        onKeyDown={(e) => {
                          if (e.key === 'Enter' || e.key === ' ') {
                            e.preventDefault();
                            openDatePicker(endDateInputRef);
                          }
                        }}
                        className={`w-full pr-9 ${theme.controlSelect} text-base cursor-pointer`}
                      />
                      <CalendarDays className="pointer-events-none absolute right-2.5 top-1/2 h-4 w-4 -translate-y-1/2 text-zinc-500 dark:text-zinc-400" />
                      <input
                        ref={endDateInputRef}
                        id="endDate"
                        name="endDate"
                        type="date"
                        value={endDate}
                        onChange={(e) => setEndDate(e.target.value)}
                        className="absolute h-0 w-0 opacity-0"
                        tabIndex={-1}
                        aria-hidden="true"
                      />
                    </div>
                  </div>
                </div>

                <div>
                  <label htmlFor="venue" className={`mb-1 block text-sm font-medium ${theme.subtext}`}>
                    Venue / Location
                  </label>
                  <input
                    id="venue"
                    name="venue"
                    type="text"
                    placeholder="e.g. Bengaluru, Karnataka or Online"
                    className={`w-full ${theme.controlSelect} text-base`}
                  />
                </div>

                <div>
                  <label htmlFor="description" className={`mb-1 block text-sm font-medium ${theme.subtext}`}>
                    Description
                  </label>
                  <textarea
                    id="description"
                    name="description"
                    rows="2"
                    placeholder="Short summary..."
                    className={`w-full ${theme.controlSelect} text-base resize-none`}
                  ></textarea>
                </div>

                <div>
                  <label htmlFor="username" className={`mb-1 block text-sm font-medium ${theme.subtext}`}>
                    Contributor Name
                  </label>
                  <input
                    id="username"
                    name="username"
                    type="text"
                    placeholder="How should we credit you?"
                    className={`w-full ${theme.controlSelect} text-base`}
                  />
                </div>

                {status === 'error' && (
                  <div className="rounded-md bg-red-50 p-3 text-sm text-red-600 dark:bg-red-900/30 dark:text-red-400">
                    {errorMessage}
                  </div>
                )}

                <button
                  type="submit"
                  disabled={loading}
                  className="w-full rounded-md bg-zinc-900 py-2.5 text-sm font-medium text-white hover:bg-zinc-800 disabled:opacity-50 dark:bg-zinc-100 dark:text-zinc-900 dark:hover:bg-zinc-200 transition-colors"
                >
                  {loading ? 'Submitting...' : 'Submit Suggestion'}
                </button>
              </form>
            )}
          </div>
        </div>
      )}
    </>
  );
}