import rasterio
import numpy as np
import matplotlib.pyplot as plt

with rasterio.open('clipped_images/jaipur_clipped_B1.TIF','r') as src:
    red_band = src.read(1)
    metadata = src.meta
    
plt.imshow(red_band, cmap='winter')
plt.colorbar()
plt.show()