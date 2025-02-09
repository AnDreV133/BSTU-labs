from finish.environment import Environment


class UseBehavior:

    def use_on(self, player, target): pass


class DistanceAttack(UseBehavior):
    def use_on(self, player, target):
        if Environment.distance_to(player, target) < 10:
            Environment.comp_hp(target, -40 if target.hp > 40 else -target.hp)


class MeleeAttack(UseBehavior):
    def use_on(self, player, target):
        if Environment.distance_to(player, target) <= 1:
            Environment.comp_hp(target, -200 if target.hp > 200 else -target.hp)
