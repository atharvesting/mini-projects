import os
from typing import Optional
import pandas as pd


class Directory:
    def __init__(self, dir_path):
        if not os.path.isdir(dir_path):
            raise FileNotFoundError("Directory not found")
        self.dir_path = dir_path

    def get_all_contents(self) -> list[tuple]:
        return list(os.walk(self.dir_path))

    def data_frame_list(self) -> dict[str, pd.DataFrame]:
        df_dic = {}
        for dirpath, dirnames, filenames in self.get_all_contents():
            for filename in filenames:
                f = File(os.path.join(dirpath, filename))
                if f.is_valid:
                    df_dic[f.file_name] = f.to_dataframe()

        return df_dic


class File:
    def __init__(self, file_path):
        self.file_path = file_path
        self.file_name = os.path.basename(self.file_path)
        self.extension = os.path.splitext(self.file_path)[1]
        self.is_valid = self.extension in {".json", ".csv"} and os.path.isfile(file_path)

    def to_dataframe(self) -> Optional[pd.DataFrame]:
        if not self.is_valid:
            return None
        try:
            if self.extension == ".json":
                return pd.read_json(self.file_path)
            elif self.extension == ".csv":
                return pd.read_csv(self.file_path)
        except Exception as e:
            print(f"Error reading {self.file_path}: {e}")
            return None

class DateData:
    def __init__(self, df: pd.DataFrame):
        pass

class ProcessedFile:
    def __init__(self, df: pd.DataFrame):
        self.df = df
        self.insight = df.describe()