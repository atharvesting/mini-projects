import itertools
from random import randint
from itertools import cycle
from typing import Optional, Callable, Literal

from ShutTheBox_Simulator.logic import mass_roller
from logic import get_distinct_parts
from strategies import (most_numbers_first,
                        least_numbers_first,
                        lowest_average_first,
                        highest_average_first)


class Player:
    """
    A class representing a player in the Shut The Box game.

    Manages player-specific state including their board numbers, unique ID,
    and selected playing strategy from predefined strategy functions.

    Class Attributes
    ----------------
    strategy_dict : dict of str to Callable
        Dictionary mapping strategy names to their corresponding strategy
        functions. Available strategies include:
        - "most_numbers_first"
        - "least_numbers_first"
        - "lowest_average_first"
        - "highest_average_first"

    Parameters
    ----------
    p_id : int
        Unique identification number for the player.
    strategy_name : str, optional
        Name of the playing strategy to use. Must be a key in `strategy_dict`.
        Defaults to "most_numbers_first".

    Raises
    ------
    ValueError
        If `strategy_name` is not found in `strategy_dict`.

    Attributes
    ----------
    nums : dict of int to bool
        Tracks the state of the player's board numbers
        (True if shut/closed, False if open).
    p_id : int
        Unique player identification number.
    strategy_name : str
        Name of the currently selected playing strategy.
    strategy_func : Callable
        Function reference for the selected strategy that determines
        optimal moves given dice roll and board state.
    """
    strategy_dict: dict[str, Callable] = {
        "most_numbers_first": most_numbers_first,
        "least_numbers_first": least_numbers_first,
        "lowest_average_first": lowest_average_first,
        "highest_average_first": highest_average_first
    }

    def __init__(self, p_id: int, strategy_name: str = "highest_average_first"):
        """
        Initialize a Player with unique ID and selected strategy.

        Args:
            p_id (int): Unique player identification number.
            strategy_name (str, optional): Strategy name. Defaults to
                "most_numbers_first".
        """
        self.nums: set[int] = set()
        self.p_id: int = p_id

        if strategy_name not in self.strategy_dict:
            raise ValueError(
                f"Choose a valid strategy name from: {list(self.strategy_dict.keys())}"
            )

        self.strategy_name = strategy_name
        self.strategy_func = self.strategy_dict[strategy_name]
        statusOptions = Literal["winner", "loser", "playing", "finished"]
        self.status: statusOptions = "playing"

    def roll_die(self) -> int:
        """Simulates a die roll by randomly returning a number between 1 and 6."""
        return randint(1, 6)

    def roll_n_dice(self, dice_count: int) -> int:
        """Simulates rolling n number of dice and returns the sum of all results."""

        '''total: int = 0
        for i in range(dice_count):
            total += self.roll_dice()

        return total'''
        return sum(self.roll_die() for _ in range(dice_count))

    def choose_valid_combos(self, combo_list: set[frozenset[int]]) -> Optional[set[frozenset[int]]]:
        valid_set = set()
        for combo in combo_list:
            for num in combo:
                if num in self.nums or num is None:
                    break
            else:
                valid_set.add(combo)

        return valid_set if valid_set else None

    def select_final_combo(self, combo_list: set[frozenset[int]]) -> Optional[frozenset[int]]:
        """Applies the player's strategy to the list of combinations and returns the single most suitable one."""
        return self.strategy_func(combo_list)

    def update_counter(self, final_combo: frozenset[int]) -> None:
        for num in final_combo:
            self.nums.add(num)

    def __repr__(self):
        return f"""
    Player {self.p_id} | Strategy: {self.strategy_name}
    Status: {self.status} | Numbers: {self.nums}
    """


class Game:
    """
    A class representing a "Shut The Box" game session.

    This class manages the overall game setup, including the number of players,
    dice configuration, and available numbers to play with. It also initializes
    player states and sets up the turn cycle and ranking system.

    Parameters
    ----------
    p_count : int, optional
        The number of players participating in the game. Must be greater than 1.
        Default is 4.
    dice_count : int, optional
        The number of dice used in the game. Must be at least 1.
        Default is 2.
    available_nums : list of int, optional
        The list of numbers available to be "shut" during the game.
        Defaults to [1, 2, 3, 4, 5, 6, 7, 8, 9, 10].

    Raises
    ------
    ValueError
        If `p_count` is less than or equal to 1.
    ValueError
        If `dice_count` is less than or equal to 0.
    ValueError
        If the maximum number in `available_nums` exceeds `6 * dice_count`.

    Attributes
    ----------
    p_count : int
        The total number of players in the game.
    dice_count : int
        The number of dice used in each turn.
    available_nums : list of int
        The list of playable numbers for the current game session.
    players : list of dict
        A list where each player is represented as a dictionary mapping
        a `Player` instance to their board state
        (a dictionary of available numbers marked True/False).
    player_cycle : itertools.cycle
        A cyclic iterator that manages player turns in sequence.
    ranking : dict of int to Player or None
        Stores player rankings after the game ends. The key represents
        the rank (1 for the winner, etc.), and the value is the corresponding
        `Player` instance or None if unassigned.

    Technical Gameplay
    ------------------
    When it is someone's turn, they roll the dice. The total result from the dice is then fed
    into a function that returns all possible sum combinations as a list of tuples. A validator method
    then checks for all combinations that is valid, i.e. containing only so-far unused numbers.
    The list of valid combinations are then passed into the strategy function that returns a chosen tuple
    accordingly. The number(s) from this chosen tuple are then recorded in the player's counting box.
    If the person's counting box becomes equal to the game's available numbers, they are counted as a
    winner in the ranking dictionary and are removed from the game.
    """

    def __init__(
            self,
            p_count: int = 4,
            dice_count: int = 2,
            available_nums: set[int] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    ):
        """Initializes a Shut The Box Game.

        Args:
            p_count (int): Number of players in the game.
            dice_count (int): Number of dice used in the game.
            available_nums (list[int]): List of numbers available for play.
        """
        if p_count <= 1:
            raise ValueError("Invalid number of players.")

        if max(available_nums) > 6 * dice_count:
            raise ValueError("Available numbers over playable limit.")

        if dice_count <= 0:
            raise ValueError("Invalid Dice Count. Must be >= 1")

        self.p_count = p_count
        self.dice_count = dice_count
        self.available_nums = available_nums
        self.players = [Player(i+1) for i in range(p_count)]
        # self.players = [
        #     Player(1, "most_numbers_first"),
        #     Player(2, "least_numbers_first"),
        #     Player(3, "highest_average_first"),
        #     Player(4, "lowest_average_first"),
        # ]
        self.player_cycle = cycle(self.players)
        self.ranking: dict[int, Optional[Player]] = {i: None for i in range(1, p_count + 1)}
        self.status_result = {
            1: "winner",
            self.p_count: "loser"
        }
        self.dice_probabilities = mass_roller(self.dice_count, 1000000)

    def __iter__(self):
        """Return the iterator object for the game class, which is an itertools.cycle."""
        return self

    def __next__(self):
        """Advance to the next valid player and play their turn."""
        if self.is_game_over():
            raise StopIteration

        # 3. Loop to find the next active player (Replaces recursion)
        # We loop until we find a player who is NOT finished.
        while True:
            # Advance the internal cycle
            player = next(self.player_cycle)

            # Update the tracker for __repr__
            self.current_player = player

            # If player is active, play their turn and return
            if player.status not in ["winner", "finished"]:
                self.play_turn(player)
                return player

            # If everyone is finished (failsafe for the while loop)
            if self.is_game_over():
                raise StopIteration


    def players_info(self):
        return [repr(player) for player in self.players]

    def __repr__(self) -> str:
        player_reprs = [repr(player) for player in self.players]
        status_lines = [
            f"Players ({len(self.players)}):",
            *player_reprs,
            f"Ranking: {dict(self.ranking)}"
        ]
        return "\n".join(status_lines)

    def play_turn(self, player):
        """
        When it is someone's turn, they roll the die. The total result from the die is then fed
        into a function that returns all possible sum combinations as a list of tuples. A validator method
        then checks for all combinations that is valid, i.e. containing only so-far unused numbers.
        The list of valid combinations are then passed into the strategy function that returns a chosen tuple
        accordingly. The number(s) from this chosen tuple are then recorded in the player's counting box.
        If the person's counting box becomes equal to the game's available numbers, they counted as a winner
        in the ranking dictionary and are removed from the game.
        """
        print(player.p_id, player.status)
        dice_result = player.roll_n_dice(self.dice_count)
        print("Dice result:", dice_result)
        if dice_result not in self.available_nums:
            return
        combo_list: set[frozenset[int]] = get_distinct_parts(dice_result)
        valid_set = player.choose_valid_combos(combo_list)
        if not valid_set:
            return
        print("Possible combos: ", valid_set)
        chosen_combo = player.select_final_combo(valid_set)
        print("Chosen combo:", chosen_combo)
        player.update_counter(chosen_combo)
        print("Player counter:", player.nums)
        if player.nums.issuperset(self.available_nums):
            for rank, p in self.ranking.items():
                if p is None:
                    self.ranking[rank] = player
                    # player.status = self.status_result[rank] if rank in self.status_result else "finished"
                    player.status = self.status_result.get(rank, "finished")
                    break

    def is_game_over(self) -> bool:
        return all(p.status in ["winner", "loser", "finished"] for p in self.players)

    def play_game(self):
        for player in self:
            pass

        print("Game Over")
        print(self.ranking)
