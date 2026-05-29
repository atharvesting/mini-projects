# Consalgo_Visualizer
> A terminal-based algorithm visualizer to understand sorting logic in real-time.

## Overview
* **Status:** ✅
* **Started:** 2026-02-14
* **Core Stack:** C++, Windows API
* **Domain:** Systems / DSA

## Objectives
To build a functional algorithm visualizer entirely using ASCII characters on a 2D console buffer, focusing on sorting algorithm step-by-step rendering.

## Tech Stack & Key Concepts
* **Languages:** C++
* **Libraries/Tools:** Windows.h (Console Buffer)
* **Key Logic/Algorithms:** Bubble Sort, Selection Sort, Linked List based state tracking for "step" history.

## Roadmap & Features
- [x] **Phase 1: Basic Rendering** - Bar-based visualization in console.
- [x] **Phase 2: Algorithm Implementation** - Bubble and Selection sort steps.
- [x] **Phase 3: Playback Controls** - Navigation through sorting steps.

## Lessons Learned + Notes
* **Technical:** Learned about managing console buffers via `Windows.h` and the importance of state management in step-by-step visualizations.
* **Process:** Using a doubly linked list to store array states for back-and-forth visualization.

## How to Run
- Change console size to 70 (columns) x 35 (rows).