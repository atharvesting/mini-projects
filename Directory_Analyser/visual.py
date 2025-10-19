import matplotlib.pyplot as plt
from repo import DirectoryRepo

class Visualizer:
    def __init__(self, path):
        self.repo = DirectoryRepo(path)

    def plot_filetype_stats(self):
        dic = self.repo.get_filetype_stats()
        x = []; y = []
        for ext, num in dic.items():
            x.append(ext); y.append(num)
        plt.pie(y, labels=x, autopct='%1.1f%%',)
        plt.axis('equal')
        plt.title("Filetype Frequency")
        plt.show()
        
        
