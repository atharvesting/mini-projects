import os, shutil

"""
This program specially takes the Workspace Folder of the MOOC.fi courses
and extracts all the personally written programs safely and quickly.
"""

og_path = r"PATH_TO_YOUR_SOURCE_DIRECTORY"
destination_path = r"PATH_TO_THE_DIRECTORY_WHERE_FILES_WILL_BE_COPIED"

def get_paths_from_source():
    dir_list = []
    for dirpath, dirnames, filenames in os.walk(og_path):
        # if directory name is "src", then get the exact path of the directory
        for dirname in dirnames:
            if dirname == "src":
                dir_list.append(os.path.join(dirpath, dirname))
                
    return dir_list

def get_files_from_paths():
    file_list = []
    dir_list = get_paths_from_source()
    for directory in dir_list:
        filenames = os.listdir(directory)
        for filename in filenames:
            if filename.endswith(".py"):
                file_list.append(os.path.join(directory, filename))
                    
    return file_list

def copy_to_destination():
    file_list = get_files_from_paths()
    os.makedirs(destination_path, exist_ok=True)
    for file_path in file_list:
        try:
            shutil.copy2(file_path, destination_path)
            print(f"Copied {file_path} to {destination_path}")
        except Exception as e:
            print(f"Failed to copy {file_path}: {e}")
    print(f"Total files copies: {len(file_list)}")

def run():
    if not os.path.exists(og_path):
        print("Source path does not exist: ", og_path)
        return
    copy_to_destination()
        
              
run()