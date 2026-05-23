import matplotlib.pyplot as plt
from repo import DirectoryRepo

class Visualizer:
    def __init__(self, path):
        self.repo = DirectoryRepo(path)
        
    def custom_xy(self):
        """
        I need to create a custom set of x and y values.
        There should only be 6 main x values, including "others"
        The remaining 5 are the most prominent file types
        I need a function that -
            - Finds total files
            - Finds 5 most prominent file types with file count
            - Packs the rest into "Others" with their total file count
        """
        return x, y

    def plot_filetype_stats(self):
        dic = self.repo.get_filetype_stats()
        x = []; y = []
        for ext, num in dic.items():
            x.append(ext)
            y.append(num)
        plt.pie(y, labels=x, autopct='%1.1f%%',)
        plt.axis('equal')
        plt.title("Filetype Frequency")
        plt.show()
        
        
