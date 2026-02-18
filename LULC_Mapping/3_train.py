import pandas as pd
from sklearn.model_selection import train_test_split
from imblearn.over_sampling import SMOTE
import matplotlib.pyplot as plt
import numpy as np

data = pd.read_csv("data.csv")
data.dropna(inplace=True)

X_labels = ["X_utm", "Y_utm", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "NDVI", "NDWI", "NDBI"]
X = data[X_labels]
y = data['class']
y = y.dropna()

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=1, test_size=0.3)

smote = SMOTE(random_state=1)
X_train_balanced, y_train_balanced, *_ = smote.fit_resample(X_train, y_train)






"""
from collections import Counter
classes = Counter(np.array(y_train_balanced))

plt.bar(list(classes.keys()), list(classes.values()))
plt.xlabel("Classes")
plt.ylabel("Datapoint count")
plt.title("After SMOTE")
plt.show()
"""
