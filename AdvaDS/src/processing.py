from typing import Optional, Callable, Literal

import pandas as pd


def validate_integrity(check: str = Literal["row", "column"]):
    def decorator(cleaner_func: Callable):
        def wrapped(self, thresh: float = 0.75, tolerance: float = 0.75):
            result = cleaner_func(thresh, tolerance)

            if result is None:
                return None

            check_axis = 1 if check == "column" else 0

            if result.shape[check_axis] < tolerance * self.df.shape[check_axis]:
                return None

            return result

        return wrapped

    return decorator


class DataCleaner:
    def __init__(self, df: pd.DataFrame):
        self.df = df
        self.shape = df.shape

    @validate_integrity(check="column")
    def remove_bad_columns(
            self, thresh: float = 0.8, tolerance: float = 0.75
    ) -> Optional[pd.DataFrame]:
        """
        Removes columns with insufficient data

        Default threshold for removing columns = >80% values missing.
        In case the number of columns reduces to under a certain percentage of the original data, the data is deemed
        unsuitable for further processing for the sake of the integrity of the program.
        """
        return self.df.dropna(axis=1, thresh=int(thresh * self.df.shape[0]))

    def remove_bad_rows(
            self, thresh: float = 0.75, tolerance: float = 0.75
    ) -> Optional[pd.DataFrame]:
        """
        Removes columns with insufficient data.

        Default = >75% values missing.
        In case the number of columns reduces to under a certain percentage of the original data, the data is deemed
        unsuitable for further processing for the sake of the integrity of the program.
        """
        updated_df = self.df.dropna(axis=0, thresh=int(thresh * self.df.shape[1]))
        if updated_df.shape[0] < tolerance * self.df.shape[0]:
            return None
        return updated_df

    @staticmethod
    def _safe_drop(cleaner_func: Callable, *args, **kwargs) -> Optional[pd.DataFrame]:
        result = cleaner_func(*args, **kwargs)
        return result if result is not None else None

    @validate_integrity("row")
    def drop_bad_rows_columns(
            self, thresh: float = 0.75, tolerance: float = 0.75
    ) -> Optional[pd.DataFrame]:
        updated_df = self._safe_drop(
            cleaner_func=self.remove_bad_columns, thresh=thresh, tolerance=tolerance
        )
        if updated_df is None:
            return None
        return self._safe_drop(
            cleaner_func=self.remove_bad_rows, thresh=thresh, tolerance=tolerance
        )

    def remove_duplicates(
            self, thresh: float = 0.75, tolerance: float = 0.75
    ) -> Optional[pd.DataFrame]:
        updated_df = self.df.drop_duplicates(keep="first")
        if updated_df.shape[0] < tolerance * self.df.shape[0]:
            return None
        return updated_df

    def run_main_cleaner(self):
        ...
