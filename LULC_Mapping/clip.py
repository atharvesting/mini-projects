"""
Clipping Methodology:
    1. Define Shape: Create a box in Lat/Lon.
    2. Open Image: Load the satellite band.
    3. Align: Convert the box to match the image's coordinate system (Meters).
    4. Cut: Slice the image using the box.
    5. Update Info: Record the new size and position.
    6. Save: Write the smaller file to disk.
"""
import rasterio  # Primary tool to read and write to satellite images (rasters)
from rasterio.mask import mask
from shapely.geometry import box  # Creates virtual geometric shapes 
import geopandas as gpd  # Pandas but for maps and other geographical stuff

# Defining coordinate bounds for the Jaipur-sized box
minx, miny = 75.69, 26.79
maxx, maxy = 75.95, 27.05

bbox = box(minx, miny, maxx, maxy)

# The box is wrapped using the specific crs which indicates that the coordinates are in standard latitude/longitude.
geo = gpd.GeoDataFrame({'geometry': [bbox]}, crs='EPSG:4326')

for i in range(1, 8):
    with rasterio.open(
        f'LC09_L2SP_147041_20260124_20260125_02_T1/LC09_L2SP_147041_20260124_20260125_02_T1_SR_B{i}.TIF'
        ) as src:  # We open the file for each available band captured to prepare it.
        
        # to_crs() is used to reproject/transform geographic data from one coordinate system (like degrees) to another (like UTM)
        # crs = Coordinate Reference System
        # This function maps the GeoDataFrame perfectly to the satellite image (the crs of the src image)
        geo_reprojected = geo.to_crs(src.crs)
        
        # Clipping process
        # clipped_image: Array of numbers representing light intensity
        # clipped_transform: Mathematical formula to indicate where the pixels (numbers) are located on the map since
            # the crop will change the positions
        clipped_image, clipped_transform = mask(
            src,                        # 1st: Satellite image 
            geo_reprojected.geometry,   # 2nd: The geometry of the reprojected/transformed GeoDataFrame
            crop=True                   # 3rd: Remove data outside the box (shrinks image dimensions)
        )
        
        # Updating image metadata
        clipped_meta = src.meta.copy()  # Make a copy of the original metadata
        clipped_meta.update({
            'height': clipped_image.shape[1],  
            'width': clipped_image.shape[2],
            'transform': clipped_transform
        })
        
        # Saving the new clipped file
        # **clipped_meta unpacks our updated ID info and applies the settings (driver, CRS, h/w, transform etc.) to the new file.
        with rasterio.open(f'clipped_images/jaipur_clipped_B{i}.TIF', 'w', **clipped_meta) as dst:
            dst.write(clipped_image)