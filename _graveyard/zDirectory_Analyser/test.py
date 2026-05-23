from repo import DirectoryRepo
from service import DirectoryService
from visual import Visualizer

path = r"C:\Users\Atharv Rawat\Desktop\Main\Archive"

dir = DirectoryRepo(path)
dirs = DirectoryService(path)
vis = Visualizer(path)

print(dir.get_condensed_stats())
'''vis.plot_filetype_stats()'''