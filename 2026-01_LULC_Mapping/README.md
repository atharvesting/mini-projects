# LULC Classification Model
> Fine-tuned program to detect and classify land use and cover for the Jaipur region.

## Overview
* **Status:** ✅
* **Started:** 2025-01-28
* **Core Stack:** Python (Rasterio, Geopandas, Sklearn), QGIS
* **Domain:** Computer Vision / GIS
* **Additional:** Intership Project for the ISA Winter Training 2025-26

## Objectives
To implement a pipeline for satellite image classification to monitor land use changes using machine learning.

## Tech Stack & Key Concepts
* **Languages:** Python
* **Libraries/Tools:** Rasterio, Geopandas, QGIS (Labelling).
* **Key Logic/Algorithms:** Random Forest, SVM, XGBoost, Feature Engineering on Landsat 9 imagery.

## Roadmap & Features

### Key steps
1. Data Sourcing: LandSat 2 images from USGS Earth Explorer
2. Data Preprocessing: Loading, Clipping and Feature Engineering (WIP)
3. Training Data Preparation: Manual labelling using QGIS
4. Model Training: Splitting, Training multiple algorithms (Random Forest, SVM, XGBoost…)
5. Model Evaluation: Eval. Metrics, Cross validation
6. Model Testing (Inference): Generate predictions and visualisation

- [x] **Phase 1: Sourcing** - LandSat images from USGS.
- [x] **Phase 2: Preprocessing** - Loading and clipping (WIP).
- [x] **Phase 3: Training & Eval** - Running algorithms and accuracy assessment.

## Lessons Learned + Notes
* **Technical:** Deeper insight into working with multi-band image data using Python.
* **Process:** Handling imbalanced LULC classes through manual labelling in QGIS.

## References & Credits
* USGS Earth Explorer for imagery