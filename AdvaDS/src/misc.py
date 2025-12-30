import os

import pandas as pd

# from .data import Directory, File
from processing import DataCleaner

#
#
df = pd.read_csv("data/melb_data.csv")

dc = DataCleaner(df)
print(dc.reduction_percentage())
