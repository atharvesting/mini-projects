import rasterio
import numpy as np
import matplotlib.pyplot as plt

bands = []
for i in range(1, 8):
    with rasterio.open(f'clipped_images/jaipur_clipped_B{i}.TIF') as src:
        bands.append(src.read(1))
        
print(bands[0].shape)

def contrast_stretch(band):
    p2, p98 = np.percentile(band, (2, 98))
    stretched = np.clip(band, p2, p98)
    return (stretched - p2) / (p98 - p2)

def scale(band):
    s = (band.astype(float) * 0.0000275) - 0.2
    return np.clip(s, 0, 1)

def process_band(band):
    final = scale(band)
    final = contrast_stretch(final)
    return final

"""# Landsat 8/9: B4=Red, B3=Green, B2=Blue
r = process_band(bands[3])
g = process_band(bands[2])
b = process_band(bands[1])

image_array = np.dstack((r, g, b))
image = plt.imshow(image_array, interpolation='nearest', aspect='equal')

plt.savefig('fig3.png', format='png', dpi=1200)
plt.axis('off')
plt.show()"""

# Feature Engineering

# NDVI (Normalized Difference Vegetation Index)
#    = (NIR - Red) / (NIR + Red)

nir = scale(bands[4])
red = scale(bands[3])

ndvi = (nir - red) / (nir + red + 1e-10)

plt.figure(figsize=(10, 8))
plt.imshow(ndvi, cmap='RdYlGn')
plt.colorbar(label='NDVI Value')
plt.title("Jaipur Vegetation Health (NDVI)")
plt.show()

# NDWI (Normalized Difference Water Index)
#   = (Green - NIR) / (Green + NIR)

green = scale(bands[2]) # Green is Band 3 in LANDSAT 9
ndwi = (green - nir) / (green + nir + 1e-10)

plt.figure(figsize=(10, 8))
plt.imshow(ndwi, cmap='RdYlGn')
plt.colorbar(label='NDVI Value')
plt.title("Jaipur Water Bodies (NDWI)")
plt.show()

# NDBI (Normalized Difference Built-Up Index)
#   = (SWIR - NIR) / (SWIR + NIR)

swir = scale(bands[5]) # SWIR is Band 6 in LANDSAT 9
ndbi = (swir - nir) / (swir + nir + 1e-10)

plt.imshow(ndbi, cmap='magma')
plt.colorbar(label='NDBI Value')
plt.title("Jaipur Urban Density (NDBI)")
plt.show()