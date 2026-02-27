import rasterio
from rasterio.transform import xy
import matplotlib.pyplot as plt
import numpy as np
from pyproj import Transformer
import pandas as pd

bands = []

for i in range(1, 8):
    with rasterio.open(f"clipped_images/jaipur_clipped_B{i}.TIF") as src:
        bands.append(src.read(1))
        if i == 1:
            meta = src.meta
            height = src.height
            width = src.width
            
stack = np.stack(bands)
print(f"Original stack shape: {stack.shape}")


X_image = stack.reshape(stack.shape[0], -1).T

band_names = [f'B{i}' for i in range(1, 8)]
final = pd.DataFrame(X_image, columns=band_names)


final['NDVI'] = (final['B5'] - final['B4']) / (final['B5'] + final['B4'] + 1e-10)
final['NDWI'] = (final['B3'] - final['B5']) / (final['B3'] + final['B5'] + 1e-10)
final['NDBI'] = (final['B6'] - final['B5']) / (final['B6'] + final['B5'] + 1e-10)

final.to_csv('image.csv')