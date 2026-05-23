# AdvaDS
> A CLI tool that automates data processing for CSV and JSON files.

## Overview
* **Status:** ✅ MVP
* **Started:** 2025-12-24
* **Core Stack:** [Python, CLI]
* **Domain:** Data Management

## Objectives
To automate data processing workflows including ingestion, cleaning, and basic exploratory analysis for tabular data.

## Tech Stack & Key Concepts
* **Languages:** Python
* **Libraries/Tools:** [e.g., Pandas, Mermaid for diagrams]
* **Key Logic/Algorithms:** Standardized cleaning steps (Missing values, Duplicates, Data types), Outlier detection, and Feature Engineering.

## Roadmap & Features
- [x] **Phase 1: Ingestion & Basic Cleaning** - Handle CSV/JSON ingestion and standard formats.
- [ ] **Phase 2: Advanced EDA** - Automated visualization and report generation.
- [ ] **Phase 3: Forecasting** - Integration of basic predictive modeling.

---

Started: 2025-12-24

```mermaid
graph TD
    A[Directory] --> B[Ingestion]
    B --> Cleaning
    subgraph Cleaning ["Cleaning"]
        subgraph Basic ["Basic Cleaning Steps"];
            direction TB
            C1[Handle Missing Values] --> C2;
            C2[Remove Duplicates] --> C3;
            C3[Correct Data Types] --> C4;
            C4[Standardize Formats]
        end
        subgraph Special ["Special Cleaning Steps"]
            direction TB
            Y[Domain-Specific Handling] --> C5;
            C5[Detect/Remove Outliers] --> C6;
            C6[Fix Inconsistencies]
        end
        Basic --> Special
    end
    Cleaning --> D[Exploratory Data Analysis];
    D --> E[Feature Engineering];
    E --> F[Model Training];
    F --> G[Evaluation];
    G --> H[Forecasting];
    H --> I
    subgraph I ["End Results"]
        B1["Report"];
        B2["Updated Dataset"];
    end
```


