import pandas as pd

df = pd.read_csv("data.csv")

c4 = df[df['class'] == 4.0]
c5 = df[df['class'] == 5.0]

print(c4.shape, c5.shape)

c4 = c4.sample(frac=0.5, random_state=1)
c5 = c5.sample(frac=0.5, random_state=1)

other = df[df['class'].isin([1.0, 2.0, 3.0])]

merged = pd.concat([other, c4, c5])

merged['class'] = merged['class'].replace(5.0, 4.0)

merged = merged.sample(frac=1, random_state=1).reset_index(drop=True)

merged.to_csv("data_2.csv")