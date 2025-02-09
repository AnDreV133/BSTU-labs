from finish.display_behavior import DisplayBehavior
from finish.item import Item


class Direction:
    UP = (0, -1)
    DOWN = (0, 1)
    LEFT = (-1, 0)
    RIGHT = (1, 0)


class Player(DisplayBehavior):
    def __init__(self, name: str, hp: int, item: Item, coord: tuple):
        self.name = name
        self.hp = hp
        self.item = item
        self.coord = coord

    def display(self):
        return f"Pl: {self.name} HP: {self.hp}"

    def use_on(self, target):
        self.item.use_on(self, target)

    def reload(self):
        self.item.reload()

    def move(self, direction: Direction, steps):
        pass
