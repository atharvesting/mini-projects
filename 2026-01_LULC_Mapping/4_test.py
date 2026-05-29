import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn import tree
from sklearn.model_selection import train_test_split
from imblearn.over_sampling import SMOTE
from matplotlib.colors import ListedColormap
import matplotlib.patches as mpatches

data = pd.read_csv("data_2.csv")
data.dropna(inplace=True)

X_labels = ["B1", "B2", "B3", "B4", "B5", "B6", "B7", "NDVI", "NDWI", "NDBI"]
X = data[X_labels]
y = data["class"]

X_train, X_test, y_train, y_test = train_test_split(
    X, y, random_state=1, test_size=0.2
)

smote = SMOTE(random_state=1)
X_train_balanced, y_train_balanced = smote.fit_resample(X_train, y_train)

dtc = tree.DecisionTreeClassifier(random_state=1)
dtc.fit(X_train_balanced, y_train_balanced)

df = pd.read_csv("image.csv")
valid_mask = df['B1'] != 0
X_valid = df.loc[valid_mask, ["B1", "B2", "B3", "B4", "B5", "B6", "B7", "NDVI", "NDWI", "NDBI"]]

full_predictions = np.zeros(len(df))
full_predictions[valid_mask] = dtc.predict(X_valid)

height = 966
width = 867
image_2d = full_predictions.reshape((height, width))

hex_colors = ['white', '#003500', '#1f77b4', '#2ca02c', '#7f7f7f']
cmap_custom = ListedColormap(hex_colors)

fig, ax = plt.subplots(figsize=(12, 10))
img_plot = ax.imshow(image_2d, cmap=cmap_custom, vmin=0, vmax=4, interpolation='none')

ax.set_title("Predicted LULC Map of Jaipur (Random Forest)", fontsize=16, pad=15)
ax.axis('off')

legend_dict = {
    'Forest (1.0)': '#003500',
    'Water (2.0)': '#1f77b4',
    'Vegetation (3.0)': '#2ca02c',
    'Impervious / Built-up (4.0)': '#7f7f7f'
}

patches = [mpatches.Patch(color=color, label=label) for label, color in legend_dict.items()]
ax.legend(handles=patches, bbox_to_anchor=(1.02, 1), borderaxespad=0., fontsize=10)

plt.tight_layout()
plt.show()