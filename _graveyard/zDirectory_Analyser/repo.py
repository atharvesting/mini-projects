import os, time

class DirectoryRepo:
    def __init__(self, dir_path: str):
        self.dir_path = dir_path
        self.extension_map = {
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
        return list(os.walk(self.dir_path))
    
    def get_file_count(self):
        return len(self.get_all_contents())

    def get_immediate_file_types(self, file_type: str):
        return [file for file in self.get_immediate_contents() if file.endswith(file_type)]
    
    def get_all_file_types(self, file_type: str):
        return [
            f
            for root, dirs, files in self.get_all_contents()
            for f in files if f.endswith(file_type)
        ]

    def get_all_files(self):
        return [
            os.path.join(root, f)
            for root, dirs, files in self.get_all_contents()
            for f in files
        ]

    def get_filetype_stats(self):
        result = {}
        for path in self.get_all_files():
            ext = os.path.splitext(path)[1].lower()
            if not ext:
                ext = '<no_extension>'
            result[ext] = result.get(ext, 0) + 1
        # Sorting key-value pairs by value, and by key if values are the same
        return sorted(result.items(), key=lambda kv: (kv[1], kv[0]), reverse=True)
    
    def find_sum(self, listi, l=None, r=None):
        total = 0
        temp = listi[l:r]
        for t in temp:
            total += t[1]
        
        return total
    
    def get_condensed_stats(self):
        top = 7
        # Result = [(top1, count), (top2, count), (top3, count), (top4, count), ("Others", count)]
        s = self.get_filetype_stats()
        result = s[:top]
        result += [("Others", self.find_sum(s, top))]
        
        """
        I need a function that -
            - Takes a valid range
            - Finds the sum of counts stored in tuples in that range
        """
        return result
        
    @staticmethod
    def timestamp_to_human(timestamp):
        return time.ctime(timestamp)
        
    def get_creation_date_for_all(self):
        return [(file, os.path.getctime(file)) for file in self.get_all_files()]