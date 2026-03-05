# Markov Predictor
> A probabilistic text prediction system using Markov chains.

## Overview
* **Status:** MVP
* **Started:** 2026-03-03
* **Core Stack:** C++
* **Domain:** NLP / DSA

## Objectives
To build a trigram (currently bigram) text prediction system that analyzes word transitions and generates coherent sequences.

## Tech Stack & Key Concepts
* **Languages:** C++
* **Libraries/Tools:** Standard Template Library (Maps, Vectors).
* **Key Logic/Algorithms:** Markov Chains, Inverse Transform Sampling, Mersenne Twister RNG.

## Roadmap & Features
- [x] **Phase 1: Bigram Model** - Core prediction logic.
- [/] **Phase 2: Trigram Support** - Higher context prediction.
- [ ] **Phase 3: CLI UI** - Improved user interface for inputting seeds.

## Lessons Learned + Notes
* **Technical:** Implementing Inverse Transform Sampling in C++ for weighted probability selection.
* **Process:** Preprocessing text involves significant edge-case handling (punctuation, casing).

## How to run

1. Place a `test.txt` file in the project directory
2. Modify the starting word in `main()` as needed
3. Compile and run the program


For more on Markov chains, see the [Wikipedia article](https://en.wikipedia.org/wiki/Markov_chain).