def find_pairs(n: int) -> list[tuple[int, int]]:
    pairs: list[tuple[int, int]] = []
    for i in range(1, int(n / 2 + 1)):
        if i != n - i:
            pairs.append((i, n - i))

    return pairs

class Node:
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None
