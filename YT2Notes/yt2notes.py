import subprocess
import sys
import re
import requests
import os
import platform
import time
from pathlib import Path
from textwrap import dedent

# --- CONFIGURATION ---
OLLAMA_MODEL = "llama3.2:3b-instruct-q4_K_M"
OLLAMA_API_URL = "http://localhost:11434/api/generate"

# Optimized for RTX 3050 (4GB VRAM)
CHUNK_SIZE = 3000 
CONTEXT_WINDOW = 2048 

YT_DLP_BIN = "yt-dlp.exe" if platform.system() == "Windows" else "yt-dlp"
YT_DLP_PATH = Path(".").resolve() / YT_DLP_BIN

# --- ANALYTICS STATE ---
STATS = {
    "start_time": 0.0,
    "end_time": 0.0,
    "total_chunks": 0,
    "successful_chunks": 0,
    "llm_time_cumulative": 0.0,
    "raw_chars": 0,
    "final_chars": 0
}

def ensure_yt_dlp():
    """Checks if yt-dlp exists locally; if not, downloads it."""
    if not YT_DLP_PATH.exists():
        print(f"--- {YT_DLP_BIN} not found. Downloading a local copy... ---")
        try:
            url = f"https://github.com/yt-dlp/yt-dlp/releases/latest/download/{YT_DLP_BIN}"
            r = requests.get(url, allow_redirects=True, timeout=30)
            YT_DLP_PATH.write_bytes(r.content)
            
            if platform.system() != "Windows":
                os.chmod(YT_DLP_PATH, 0o755)
            print("--- Download complete. ---\n")
        except Exception as e:
            print(f"Error downloading yt-dlp: {e}")
            sys.exit(1)

def run_local_ytdlp(args):
    """Runs the local yt-dlp binary."""
    cmd = [str(YT_DLP_PATH)] + args
    subprocess.run(cmd, check=True)

def extract_captions(url):
    ensure_yt_dlp()
    print(f"--- Extracting captions from {url} ---")
    
    # Clean up old vtt files to prevent confusion, but strictly only ones that match pattern
    # (Optional: You might want to disable this if you are queuing multiple videos)
    for old_vtt in Path(".").glob("*.en.vtt"):
        old_vtt.unlink()

    try:
        run_local_ytdlp([
            "--write-auto-sub",
            "--skip-download",
            "--sub-lang", "en",
            "--sub-format", "vtt",
            "--output", "%(id)s.%(ext)s", # Clean filename
            url
        ])
    except subprocess.CalledProcessError:
        print("Error: yt-dlp failed. Is the URL valid?")
        sys.exit(1)

    vtt_files = list(Path(".").glob("*.en.vtt"))
    if not vtt_files:
        raise FileNotFoundError("Could not find the downloaded caption file.")
    return vtt_files[0]

def clean_vtt(path):
    # --- PHASE 1: PRE-PROCESSING ---
    content = path.read_text(encoding='utf-8')
    
    # Remove header timestamps
    content = re.sub(r"\d{2}:\d{2}:\d{2}\.\d{3} --> .*?(\n|$)", " ", content)
    # Remove internal tags like <c.color> or <00:00:01>
    content = re.sub(r"<[^>]+>", "", content)
    # Remove Metadata
    content = re.sub(r"WEBVTT|Kind:.*|Language:.*|align:.*|position:.*", "", content)
    
    lines = [line.strip() for line in content.splitlines() if line.strip()]

    # --- PHASE 2: PREFIX REMOVAL ---
    unique_lines = []
    for i in range(len(lines) - 1):
        current = lines[i]
        next_line = lines[i+1]
        
        if len(current) > 2 and next_line.startswith(current):
            continue
            
        unique_lines.append(current)
    
    if lines:
        unique_lines.append(lines[-1])

    # --- PHASE 3: ROLLING MERGE ---
    return merge_rolling_captions(unique_lines)

def merge_rolling_captions(lines):
    if not lines:
        return ""
    
    merged_text = lines[0]
    
    for next_line in lines[1:]:
        context = merged_text[-min(len(merged_text), 100):]
        
        overlap_len = 0
        limit = min(len(context), len(next_line))
        
        for i in range(limit, 0, -1):
            if context.endswith(next_line[:i]):
                overlap_len = i
                break
        
        new_content = next_line[overlap_len:].strip()
        
        if new_content:
            if merged_text and merged_text[-1] not in " -" and new_content[0] not in " .,?!":
                merged_text += " "
            merged_text += new_content
            
    return merged_text

def chunk_text(text, size):
    return [text[i:i+size] for i in range(0, len(text), size)]

def save_debug_chunks(chunks):
    """Saves pre-LLM chunks to a file so you can see exactly what the AI sees."""
    debug_path = Path("debug_chunks.md")
    
    with open(debug_path, "w", encoding="utf-8") as f:
        f.write("# DEBUG: RAW CHUNKS\n\n")
        for i, chunk in enumerate(chunks, 1):
            f.write(f"## --- CHUNK {i} ({len(chunk)} chars) ---\n")
            f.write(chunk)
            f.write("\n\n")
            
    print(f"🔍 Debug: Intermediate chunks saved to {debug_path.resolve()}")

def ollama_generate(prompt, retries=3):
    """Robust Ollama caller with retry logic."""
    payload = {
        "model": OLLAMA_MODEL,
        "prompt": prompt,
        "stream": False,
        "options": {
            "num_ctx": CONTEXT_WINDOW,
            "num_gpu": 99,       
            "low_vram": True,    
            "temperature": 0.1   
        }
    }

    for attempt in range(retries):
        try:
            r = requests.post(OLLAMA_API_URL, json=payload, timeout=300)
            r.raise_for_status()
            response_json = r.json()
            return response_json.get("response", "")
            
        except requests.exceptions.ReadTimeout:
            print(f"  !! Timeout (Attempt {attempt+1}/{retries}). Model is slow. Retrying...")
            time.sleep(2) 
        except requests.exceptions.ConnectionError:
            print(f"  !! Connection Error (Attempt {attempt+1}/{retries}). Is Ollama running?")
            time.sleep(5)
        except Exception as e:
            print(f"  !! Unexpected Error: {e}")
            break
            
    return None 

def summarize_chunks(chunks):
    full_notes = []
    last_chunk_tail = ""
    STATS["total_chunks"] = len(chunks)
    
    # Checkpoint file
    progress_file = Path("notes_progress.md")
    if progress_file.exists():
        progress_file.unlink() 

    print(f"--- Processing {STATS['total_chunks']} text chunks ---")

    for i, chunk in enumerate(chunks, 1):
        print(f"Processing Section {i}/{STATS['total_chunks']}...", end=" ", flush=True)
        
        bridge_context = last_chunk_tail if last_chunk_tail else "Start of content"

        prompt = dedent(f"""
            ROLE: Technical Textbook Editor.
            TASK: Rewrite the transcript snippet into a formal, objective academic text.
            
            INPUT CONTEXT: ...{bridge_context[-200:]}
            INPUT TRANSCRIPT:
            "{chunk}"
            
            WRITING RULES:
            1. STYLE: Encyclopedic, dry, and direct. No "In this video" or "I will show you".
            2. FORMAT: Markdown headers (###), bold terms, and bullet points.
            3. DEFINITIONS: If a complex term appears (e.g., "NDVI", "API") without definition, insert one as > **[Context Note]:** ...
            4. FILTER: If conversational filler, return "NO_DATA".
            5. RESPECT THE SOURCE: The source chunks have ultimate precedence over the generated content.
            """)

        # --- TIMER START ---
        chunk_start = time.time()
        response = ollama_generate(prompt)
        chunk_duration = time.time() - chunk_start
        # --- TIMER END ---

        if response:
            if "NO_DATA" not in response and len(response) > 20:
                print(f"Done ({chunk_duration:.1f}s).")
                full_notes.append(response)
                
                # Update Stats
                STATS["successful_chunks"] += 1
                STATS["llm_time_cumulative"] += chunk_duration
                
                with open(progress_file, "a", encoding="utf-8") as f:
                    f.write(response)
                    f.write("\n\n") 
            else:
                print(f"Skipped (Non-technical / {chunk_duration:.1f}s).")
        else:
            print("FAILED.")
        
        last_chunk_tail = chunk[-400:] 

    return "\n\n".join(full_notes)

def consolidate_notes(notes_text):
    if len(notes_text) > 12000:
        print("--- Notes too long for single-pass consolidation. Saving raw output. ---")
        return notes_text

    print("--- Consolidating final notes ---")
    prompt = dedent(f"""
    Refine these lecture notes into a single cohesive Markdown document.
    Remove redundant headers and ensure smooth transitions.
    
    NOTES:
    {notes_text}
    """)
    
    refined = ollama_generate(prompt)
    return refined if refined else notes_text

def print_analytics(vtt_path):
    duration = STATS["end_time"] - STATS["start_time"]
    minutes = int(duration // 60)
    seconds = int(duration % 60)
    
    # Compression Calc
    if STATS["raw_chars"] > 0:
        compression = (1 - (STATS["final_chars"] / STATS["raw_chars"])) * 100
    else:
        compression = 0

    # Average LLM Speed
    avg_speed = 0
    if STATS["successful_chunks"] > 0:
        avg_speed = STATS["llm_time_cumulative"] / STATS["successful_chunks"]

    print("\n" + "="*40)
    print(f" 📊 PROCESS ANALYTICS")
    print("="*40)
    print(f"⏱️  Total Time:       {minutes}m {seconds}s")
    print(f"⚡ Avg Chunk Time:   {avg_speed:.2f}s")
    print(f"📄 Raw Characters:   {STATS['raw_chars']:,}")
    print(f"📝 Final Characters: {STATS['final_chars']:,}")
    print(f"📉 Compression:      {compression:.1f}% reduction")
    print(f"💾 Subtitle File:    {vtt_path}")
    print("="*40 + "\n")

def main(url):
    # FIX: Initialize floats as 0.0 to satisfy Pylance
    STATS["start_time"] = time.time()
    STATS["end_time"] = 0.0
    STATS["llm_time_cumulative"] = 0.0 
    
    # 1. Get Caption File
    try:
        vtt_path = extract_captions(url)
    except Exception as e:
        print(e)
        return

    # 2. Process Text
    raw_text = clean_vtt(vtt_path)
    STATS["raw_chars"] = len(raw_text)
    
    if len(raw_text) < 50:
        print("Error: Transcript is too short or empty.")
        return

    # 3. Chunk
    chunks = chunk_text(raw_text, CHUNK_SIZE)
    
    # --- NEW: Save chunks for inspection ---
    save_debug_chunks(chunks) 
    # ---------------------------------------

    # 4. Summarize
    notes = summarize_chunks(chunks)

    if not notes:
        print("No notes were generated.")
        return

    # 5. Final Cleanup
    final_output = consolidate_notes(notes)
    STATS["final_chars"] = len(final_output)

    # 6. Save
    out_path = Path("final_notes.md")
    out_path.write_text(final_output, encoding="utf-8")
    
    STATS["end_time"] = time.time()
    
    print(f"\nSUCCESS: Notes saved to {out_path.resolve()}")
    print_analytics(vtt_path)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python yt2notes.py <youtube_url>")
        sys.exit(1)
    main(sys.argv[1])