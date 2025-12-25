from random import randint
from itertools import cycle
from typing import Optional, Callable
from strategies import (most_numbers_first,
                        least_numbers_first,
                        lowest_average_first,
                        highest_average_first)

class Player:

    strategy_dict: dict[str, Callable] = {
        "most_numbers_first": most_numbers_first,
        "least_numbers_first": least_numbers_first,
        "lowest_average_first": lowest_average_first,
        "highest_average_first": highest_average_first
    }

    def __init__(self, p_id: int, strategy_name: str = "most_numbers_first"):
        self.nums: dict[int, bool] = {}
        self.id = id
        self.strategy_name = strategy_name
        self.strategy_func = self.strategy_dict[self.strategy_name]

    def roll_dice(self) -> int:
        return randint(1, 6)

    def roll_n_dices(self, dice_count: int) -> int:
        """total: int = 0
        for i in range(dice_count):
            total += self.roll_dice()

        return total"""
        return sum(self.roll_dice() for _ in range(dice_count))

    def select_combination(self, combination_list: list[tuple[int]]) -> Optional[tuple[int]]:
        return self.strategy_func(combination_list)

    def __str__(self):
        return f"Player {self.id} | Strategy: {self.strategy_name}"


class Game:
    def __init__(
            self,
            p_count: int = 4,
            dice_count: int = 2,
            available_nums: list[int] = [1,2,3,4,5,6,7,8,9,10]
    ):
        if p_count <= 1:
            raise ValueError("Invalid number of players.")

        if max(available_nums) > 6 * dice_count:
            raise ValueError("Available numbers over playable limit.")

        if dice_count <= 0:
            raise ValueError("Invalid Dice Count. Must be >= 1")

        self.p_count = p_count
        self.dice_count = dice_count
        self.available_nums = available_nums
        self.players = [{Player(i + 1): {n: False for n in self.available_nums}} for i in range(self.p_count)]
        self.player_cycle = cycle(self.players)
        self.ranking: dict[int, Optional[Player]] = {i: None for i in range(1, p_count+1)}

    def __iter__(self):
        return cycle(self.players)

    def __next__(self):
        return next(self.player_cycle)

    def play_turn(self, player: Player):
        if sorted(player.nums) == self.available_nums:
            return None
        dice_result = player.roll_n_dices(self.dice_count)
        combination_list = ...

        # The selected_combo is assumed to be valid, i.e. consisting of only unused numbers
        selected_combo = player.select_combination(combination_list)
        for i in selected_combo:

            ...

"""
When it is someone's turn, they roll the die. The total result from the die is then fed 
into a function that returns all possible sum combinations as a list of tuples. A validator method
then checks for all combinations that is valid, i.e. containing only so-far unused numbers.
The list of valid combinations are then passed into the strategy function that returns a chosen tuple
accordingly. The number(s) from this chosen tuple are then recorded in the player's counting box.
If the person's counting box becomes equal to the game's available numbers, they counted as a winner
in the ranking dictionary and are removed from the game.

"""



