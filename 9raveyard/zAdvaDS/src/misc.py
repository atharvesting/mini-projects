import os

import pandas as pd

# from .data import Directory, File
from processing import DataCleaner

df = pd.read_csv("data/melb_data.csv")

dc = DataCleaner(df)
print(dc.drop_bad_rows_columns())
print(dc.df.shape)
