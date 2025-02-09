from begin.use_behavior import DistanceAttack, UseBehavior
from begin.display_behavior import DisplayAK74, DisplayTT, DisplayBehavior


class Item(UseBehavior, DisplayBehavior):
    def __init__(self, use_behavior, display_behavior):
        self.use_behavior = use_behavior
        self.display_behavior = display_behavior

    def use_on(self, player, target):
        self.use_behavior.use_on(player, target)

    def display(self):
        return self.display_behavior.display()


class AK74(Item):
    def __init__(self):
        super().__init__(DistanceAttack(), DisplayAK74())


class TT(Item):
    def __init__(self):
        super().__init__(DistanceAttack(), DisplayTT())
