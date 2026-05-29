import os, sys, time
from repo import DirectoryRepo
    
class DirectoryService:
    def __init__(self, dir_path: str):
        self.repo = DirectoryRepo(dir_path)
        self.dir_path = dir_path
    
    def check_get_immediate_file_type(self, file_type:str):
        if not file_type.startswith("."):
            return None
        return self.repo.get_immediate_file_types(file_type)
    
    
    
    
    '''def print_get_all_file_types(self, file_type):
        for file in self.repo.get_all_file_types(file_type):
            print(file)
            
    def print_get_immediate_file_type(self, file_type):
        for file in self.repo.get_immediate_file_types(file_type):
            print(file)'''
        