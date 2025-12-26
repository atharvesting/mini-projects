from ShutTheBox_Simulator.game_entities import Player


def find_pairs(n: int) -> list[tuple[int, int]]:
    '''
    pairs: list[tuple[int, int]] = []
    for i in range(1, int(n / 2 + 1)):
        if i != n - i:
            pairs.append((i, n - i))

    return pairs
    '''
    return [(i, n-i) for i in range(1, n) if i != n-i]

def get_distinct_parts(n: int) -> set[frozenset[int]]:
    def helper(remaining: int, used: set[int], path: set[int]) -> None:
        if remaining == 0:
            result.add(frozenset(path))  # frozenset for hashability
            return
        for i in range(1, remaining + 1):
            if i not in used:
                path.add(i)
                helper(remaining - i, used | {i}, path)
                path.remove(i)  # backtrack

    result = set()
    helper(n, set(), set())

    return result

def mass_roller(dice_count, roll_count):
    num_dic = {i+1: 0 for i in range(dice_count * 6)}
    p = Player(1)
    for i in range(roll_count):
        num_dic[p.roll_n_dice(dice_count)] += 1

    for key, value in num_dic.items():
        num_dic[key] = value / roll_count

    return num_dic