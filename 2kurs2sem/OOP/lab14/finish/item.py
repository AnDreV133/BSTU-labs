from finish.ammunition_behavior import AmmunitionBehavior, AmmunitionOn30, AmmunitionOn8, NoAmmunition
from finish.use_behavior import DistanceAttack, UseBehavior, MeleeAttack
from finish.display_behavior import DisplayAK74, DisplayTT, DisplayBehavior, DisplayKnife


class Item(UseBehavior, DisplayBehavior, AmmunitionBehavior):
    def __init__(self, use_behavior, display_behavior, ammunition_behavior):
        self.use_behavior = use_behavior
        self.display_behavior = display_behavior
        self.ammunition_behavior = ammunition_behavior

    def use_on(self, player, target):
        if self.ammunition_behavior.spend_ammo():
            self.use_behavior.use_on(player, target)

    def display(self):
        return self.display_behavior.display()

    def reload(self):
        self.ammunition_behavior.reload()


class AK74(Item):
    def __init__(self):
        super().__init__(DistanceAttack(), DisplayAK74(), AmmunitionOn30())


class TT(Item):
    def __init__(self):
        super().__init__(DistanceAttack(), DisplayTT(), AmmunitionOn8())


class Knife(Item):
    def __init__(self):
        super().__init__(MeleeAttack(), DisplayKnife(), NoAmmunition())
