from typing import Optional
from ShutTheBox_Simulator.logic import mass_roller


def most_numbers_first(combo_list: set[frozenset[int]]) -> Optional[frozenset[int]]:
    return max(combo_list, key=len) if combo_list else None

def least_numbers_first(combo_list: set[frozenset[int]]) -> Optional[frozenset[int]]:
    return min(combo_list, key=len) if combo_list else None

def highest_average_first(combo_list: set[frozenset[int]]) -> Optional[frozenset[int]]:
    return max(combo_list, key=lambda combo: sum(combo) / len(combo)) if combo_list else None

def lowest_average_first(combo_list: set[frozenset[int]]) -> Optional[frozenset[int]]:
    """result = None
    if combination_list:
        min_avg = 0
        for combination in combination_list:
            avg = sum(combination) / len(combination)
            if avg < min_avg:
                min_avg = avg
                result = combination
    return result"""
    return min(combo_list, key=lambda combo: sum(combo)/len(combo)) if combo_list else None

def rarest_first(combo_list: set[frozenset[int]], dice_count: int = 2) -> Optional[frozenset[int]]:
    p_dict = mass_roller(dice_count, 10000)