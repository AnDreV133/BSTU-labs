from pytest import *

from begin.item import *
from begin.player import Player
from begin.window import *


@mark.parametrize("players_team_first, players_team_second",
                  [
                      (
                              [Player("C", 150, AK74(), (6, 7))],
                              [Player("A", 90, AK74(), (0, 0)),
                               Player("B", 100, TT(), (0, 2))]
                      )
                  ], )
def test_add_players(players_team_first, players_team_second):
    window = Window()
    env = window.get_environment()

    for player in players_team_first:
        env.add_player_to_team_first(player)

    for player in players_team_second:
        env.add_player_to_team_second(player)

    assert env.team_first == players_team_first
    assert env.team_second == players_team_second


@mark.parametrize("player, result",
                  [
                      (
                              Player("C", 150, AK74(), (6, 7)),
                              f"Pl: C HP: 150",
                      ),
                      (
                              Player("E", 100, TT(), (6, 7)),
                              f"Pl: E HP: 100",
                      )
                  ])
def test_display_player(player, result):
    assert player.display() == result


@mark.parametrize("player, result",
                  [
                      (
                              Player("C", 150, AK74(), (6, 7)),
                              "AK74",
                      ),
                      (
                              Player("E", 100, TT(), (6, 7)),
                              "TT",
                      )
                  ])
def test_display_item_of_player(player, result):
    assert player.item.display() == result


@mark.parametrize("attacker, defender, result",
                  [
                      (
                              Player("C", 150, AK74(), (6, 7)),
                              Player("E", 100, TT(), (6, 7)),
                              60,
                      ),
                      (
                              Player("C", 150, AK74(), (6, 7)),
                              Player("E", 10, TT(), (6, 7)),
                              0,
                      )
                  ])
def test_attack(attacker, defender, result):
    attacker.use_on(defender)
    assert defender.hp == result
