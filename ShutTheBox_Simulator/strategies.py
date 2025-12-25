from typing import Optional

def most_numbers_first(combination_list: list[tuple]) -> Optional[tuple]:
    """result = None
    if combination_list:
        max_len = 0
        for combination in combination_list:
            combi_length = len(combination)
            if combi_length > max_len:
                max_len = combi_length
                result = combination
    return result"""
    return max(combination_list, key=len) if combination_list else None


def least_numbers_first(combination_list: list[tuple]) -> Optional[tuple]:
    """result = None
    if combination_list:
        min_len = len(combination_list[0])
        for combination in combination_list:
            combi_length = len(combination)
            if combi_length < min_len:
                min_len = combi_length
                result = combination
    return result"""
    return min(combination_list, key=len) if combination_list else None


def highest_average_first(combination_list: list[tuple[int]]) -> Optional[tuple[int]]:
    """result = None
    if combination_list:
        max_avg = 0
        for combination in combination_list:
            avg = sum(combination) / len(combination)
            if avg > max_avg:
                max_avg = avg
                result = combination
    return result"""
    return max(combination_list, key=lambda combo: sum(combo) / len(combo)) if combination_list else None


def lowest_average_first(combination_list: list[tuple[int]]) -> Optional[tuple[int]]:
    """result = None
    if combination_list:
        min_avg = 0
        for combination in combination_list:
            avg = sum(combination) / len(combination)
            if avg < min_avg:
                min_avg = avg
                result = combination
    return result"""
    return min(combination_list, key=lambda combo: sum(combo)/len(combo)) if combination_list else None