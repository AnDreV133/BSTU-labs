import math


class Environment:
    def __init__(self):
        self.team_first = []
        self.team_second = []

        self.battleMap = []

    @staticmethod
    def distance_to(player, target):
        return math.sqrt((player.coord[0] - target.coord[0]) ** 2 + (player.coord[1] - target.coord[1]) ** 2)

    @staticmethod
    def comp_hp(player, delta_hp):
        player.hp += delta_hp

    def add_player_to_team_first(self, player):
        self.add_player_to_team(player, self.team_first)

    def add_player_to_team_second(self, player):
        self.add_player_to_team(player, self.team_second)

    @staticmethod
    def add_player_to_team(player, team: list):
        team.append(player)

    def remove_player_from_team_first(self, player):
        self.remove_player_from_team(player, self.team_first)

    def remove_player_from_team_second(self, player):
        self.remove_player_from_team(player, self.team_second)

    @staticmethod
    def remove_player_from_team(player, team: list):
        team.remove(player)

    def move_player(self, player, coord: tuple):
        pass
