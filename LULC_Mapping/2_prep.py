import rasterio
import numpy as np
import matplotlib.pyplot as plt
from pyproj import Transformer
import pandas as pd

coord_data = pd.read_csv('training_data_points.csv')

# Load the data of all bands
bands = []
for i in range(1, 8):
    with rasterio.open(f'clipped_images/jaipur_clipped_B{i}.TIF') as src:
        bands.append(src.read(1))
        if i == 1:
            meta = src.crs
            transform = src.transform
         
stack = np.stack(bands)

tf = Transformer.from_crs("EPSG:4326", "EPSG:32643", always_xy=True)
coord_data['X_utm'], coord_data['Y_utm'] = tf.transform(
    coord_data['X'].values, 
    coord_data['Y'].values
)

with rasterio.open('clipped_images/jaipur_clipped_B1.TIF') as src:
    x_coords = coord_data['X_utm'].values
    y_coords = coord_data['Y_utm'].values
    
    from rasterio.transform import rowcol
    rows, cols = rowcol(transform, x_coords, y_coords)
    rows = np.array(rows).astype(int)
    cols = np.array(cols).astype(int)
    
pixel_values = stack[:, rows, cols].T

band_names = [f'B{i}' for i in range(1, 8)]
spectral_df = pd.DataFrame(pixel_values, columns=band_names)

final = pd.concat([coord_data, spectral_df], axis=1)  

final['NDVI'] = (final['B5'] - final['B4']) / (final['B5'] + final['B4'] + 1e-10)
final['NDWI'] = (final['B3'] - final['B5']) / (final['B3'] + final['B5'] + 1e-10)
final['NDBI'] = (final['B6'] - final['B5']) / (final['B6'] + final['B5'] + 1e-10)


final.to_csv('data.csv')
