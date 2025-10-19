import os, sys, time

class DirectoryRepo:
    def __init__(self, dir_path: str):
        self.dir_path = dir_path
        
    extension_map = {
        '.txt': 'Text file',
        '.pdf': 'PDF file',
        '.doc': 'Microsoft Word document',
        '.docx': 'Microsoft Word document',
        '.xls': 'Microsoft Excel spreadsheet',
        '.xlsx': 'Microsoft Excel spreadsheet',
        '.ppt': 'Microsoft PowerPoint presentation',
        '.pptx': 'Microsoft PowerPoint presentation',
        '.jpg': 'JPEG image',
        '.jpeg': 'JPEG image',
        '.png': 'PNG image',
        '.gif': 'GIF image',
        '.csv': 'Comma-separated values file',
        '.zip': 'ZIP archive',
        '.rar': 'RAR archive',
        '.mp3': 'MP3 audio file',
        '.wav': 'WAV audio file',
        '.mp4': 'MP4 video file',
        '.html': 'Hypertext Markup Language file',
        '.py': 'Python script file',
        # Programmer-specific extensions
        '.js': 'JavaScript file',
        '.java': 'Java source file',
        '.c': 'C source file',
        '.cpp': 'C++ source file',
        '.cs': 'C# source file',
        '.rb': 'Ruby script file',
        '.php': 'PHP script file',
        '.go': 'Go source file',
        '.swift': 'Swift source file',
        '.ts': 'TypeScript file',
    }
        
    def get_immediate_contents(self):
        return os.listdir(self.dir_path)
    
    def get_all_contents(self):
        return [content for content in os.walk(self.dir_path)]
    
    def get_immediate_file_types(self, file_type: str):
        return [file for file in self.get_immediate_contents() if file.endswith(file_type)]
    
    def get_all_file_types(self, file_type: str):
        return [
            file
            for root, dir, files in self.get_all_contents()
            for file in files if file.endswith(file_type)
        ]
    
    def get_all_files(self):
        return [
            os.path.join(root, file)
            for root, dir, files in self.get_all_contents()
            for file in files
        ]
        
    def get_filetype_stats(self):
        result = {}
        for file in self.get_all_files():
            strs = "." + os.path.splitext(file)[1].split(".")[1]
            if strs not in result:
                result[strs] = 1
            else:
                result[strs] += 1
        
        return result
            
        
    @staticmethod
    def timestamp_to_human(timestamp):
        return time.ctime(timestamp)
        
    def get_creation_date_for_all(self):
        return [(file, os.path.getctime(file)) for file in self.get_all_files()]