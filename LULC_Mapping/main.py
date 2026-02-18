from collections import Counter
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data.csv")

classes = Counter(df['class'])

plt.bar(list(classes.keys()), list(classes.values()))
plt.xlabel("Classes")
plt.ylabel("Datapoint count")
plt.title("Class Imbalance in Data")
plt.show()

