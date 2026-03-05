from typing import Optional, Callable, Literal

import pandas as pd


def validate_integrity(check: str = Literal["row", "column"]):
    def decorator(cleaner_func: Callable):
        def wrapped(self, *args, **kwargs):
            cleaner_func(self, *args, **kwargs)

            check_axis = 1 if check == "column" else 0
            tolerance = kwargs.get("tolerance", 0.75)

            current_size = self.df.shape[check_axis]
            original_size = self._og.shape[check_axis]

            if current_size < (tolerance * original_size):
                print(
                    f"Integrity check failed: {check} count dropped below {tolerance * 100}%"
                )
                return None
            return None

        return wrapped

    return decorator


class DataCleaner:
    def __init__(self, df: pd.DataFrame):
        self.df: pd.DataFrame = df
        self._og: pd.DataFrame = self.df.copy()
        self.shape: tuple[int, int] = df.shape

    @validate_integrity(check="column")
    def remove_bad_columns(self, thresh: float = 0.8, tolerance: float = 0.75):
        """
        Removes columns with insufficient data

        Default threshold for removing columns = >80% values missing.
        In case the number of columns reduces to under a certain percentage of the original data, the data is deemed
        unsuitable for further processing for the sake of the integrity of the program.
        """
        self.df.dropna(axis=1, thresh=int(thresh * self.df.shape[0]), inplace=True)

    @validate_integrity(check="row")
    def remove_bad_rows(self, thresh: float = 0.9, tolerance: float = 0.75):
        """
        Removes rows with insufficient data.

        Default = Remove row if <90% values available.
        In case the number of rows reduces to under a certain percentage of the original data, the data is deemed
        unsuitable for further processing for the sake of the integrity of the program.
        """
        self.df.dropna(axis=0, thresh=int(thresh * self.df.shape[1]), inplace=True)

    def drop_bad_rows_columns(self, thresh: float = 0.75, tolerance: float = 0.75):
        self.remove_bad_rows(thresh=thresh, tolerance=tolerance)
        self.remove_bad_columns(thresh=thresh, tolerance=tolerance)

    @validate_integrity(check="row")
    def remove_duplicates(self, tolerance: float = 0.75):
        updated_df = self.df.drop_duplicates(keep="first", inplace=True)

    def run_main_cleaner(self): ...
