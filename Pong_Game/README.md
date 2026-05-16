# Pong Game
> A single-paddle, pong-style console game where you catch falling balls and survive chaotic power-ups.

![Ball Fall](/1_Assets/Pong_Game.png)

## Overview
* **Status:** MVP
* **Started:** 2026-04-10
* **Core Stack:** C++ (C++20), Windows Console API
* **Domain:** Game Dev / Systems
* **Platform:** Windows only (uses Windows Console APIs)

## Objectives
Build a real-time console arcade game with a smooth game loop, responsive input, collision logic, and a modular power-up system.

## Tech Stack & Key Concepts
* **Languages:** C++
* **Libraries/Tools:** Windows.h, chrono, random, std::format
* **Key Logic/Algorithms:** Console screen buffer rendering, delta-time movement, collision detection, power-up lifecycle with callbacks.

## Roadmap & Features
- [x] **Phase 1: Core Loop** - Paddle controls, falling ball, score.
- [x] **Phase 2: Modifiers** - Score multipliers, speed tweaks, bar size changes.
- [x] **Phase 3: Power-ups** - Magnet/anti-magnet, blessings, bombs, disguised drops.

## Controls
- `A` / `D` - Move bar left/right
- `Q` - Quit

## Lessons Learned + Notes
* **Technical:** Using a delta-time loop keeps gameplay consistent across machines.
* **Process:** Callback-based power-up activation made effects easy to add and remove.

## How to Run
### Option A: Visual Studio (recommended)
1. Open `Pong_Game.slnx` in Visual Studio 2022.
2. Build for x64 and run.

### Option B: Command line (Windows)
**MSVC (Developer PowerShell):**
```powershell
cl /std:c++20 /EHsc Pong_Game.cpp /link user32.lib
Pong_Game.exe
```

**MinGW g++:**
```bash
g++ Pong_Game.cpp -std=c++20 -O2 -o Pong_Game.exe -luser32
./Pong_Game.exe
```