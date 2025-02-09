class AmmunitionBehavior:

    # def curr_ammo(self): pass
    #
    # def max_ammo(self): pass
    #
    # def get_curr_ammo(self):
    #     return self.curr_ammo()
    #
    # def get_max_ammo(self):
    #     return self.max_ammo()

    def spend_ammo(self) -> bool: pass

    # if self.curr_ammo() > 0:
    #     # self.curr_ammo() -= 1
    #     return True
    #
    # return False

    def reload(self): pass


class AmmunitionOn30(AmmunitionBehavior):
    curr_ammo = 30
    max_ammo = 30

    def spend_ammo(self):
        if self.curr_ammo > 0:
            self.curr_ammo -= 1
            return True

        return False

    def reload(self):
        self.curr_ammo = self.max_ammo


class AmmunitionOn8(AmmunitionBehavior):
    curr_ammo = 8
    max_ammo = 8

    def spend_ammo(self):
        if self.curr_ammo > 0:
            self.curr_ammo -= 1
            return True

        return False

    def reload(self):
        self.curr_ammo = self.max_ammo


class NoAmmunition(AmmunitionBehavior):
    def spend_ammo(self):
        return True

    def reload(self):
        pass
