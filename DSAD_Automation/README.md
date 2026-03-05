# DSA Diary Automation
> Automating "Learn In Public" activities by documenting DSA solutions via LLMs.

## Overview
* **Status:** WIP
* **Started:** 2025-12-24
* **Core Stack:** Python, GitHub Actions
* **Domain:** Automation / LLM

## Objectives
To create a diary entry for every DSA problem solved using an LLM, then auto-posting to a personal website and social media. 

This automation is part of my process to automate my Learn In Public activities so that I can make more time for high-value learning and practice work.

I maintain a personal repository where I save my solutions for DSA problems I solve. I am probably the millionth person to do this, but the value-added extension to this practice is this automation which allows me to use an LLM to document a kind of diary entry for each problem I solve, and then a. Post it to my personal website and b. Post promotional content pointing to the diary.

## Tech Stack & Key Concepts
* **Languages:** Python
* **Libraries/Tools:** Google Gemini API, GitHub Actions, Regular Expressions.
* **Key Logic/Algorithms:** System prompt engineering, Automated workflow triggers via GitHub Secrets.

## Roadmap & Features
- [x] **Phase 1: MVP** - Generate blog posts from source code (Done).
- [/] **Phase 2: Integration** - Auto-send posts to website (WIP).
- [ ] **Phase 3: Supplementary** - AI-generated promotional posts on X (TODO).

## Lessons Learned + Notes
* **Technical:** Learned about environment variables (.env), GitHub Secrets, and API integration.
* **Process:** Streamlining the "Learn In Public" workflow to focus on high-value practice.

## How to Run
```bash
# Triggered via GitHub Actions on push, or locally:
python main.py
```