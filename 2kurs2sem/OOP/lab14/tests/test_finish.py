from pytest import *

from finish.item import *
from finish.player import Player
from finish.window import *


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


@mark.parametrize("player, target, result",
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
                      ),
                      (
                              Player("C", 150, AK74(), (0, 0)),
                              Player("E", 10, TT(), (20, 20)),
                              10,
                      )
                  ])
def test_attack(player, target, result):
    player.use_on(target)
    assert target.hp == result


@mark.parametrize("player, target, result",
                  [(
                          Player("C", 150, Knife(), (7, 7)),
                          Player("E", 100, AK74(), (7, 7)),
                          0
                  )]
                  )
def test_knife_attack(player, target, result):
    player.use_on(target)
    assert target.hp == result


@mark.parametrize("player, result",
                  [(
                          Player("C", 150, Knife(), (6, 7)),
                          "Knife"
                  )]
                  )
def test_knife_display(player, result):
    assert player.item.display() == result


@mark.parametrize("player",
                  [(
                          Player("A", 150, AK74(), (6, 7))
                  )]
                  )
def test_reload(player):
    temp_player = Player("C", 100, AK74(), (6, 7))
    while player.item.ammunition_behavior.curr_ammo > 0:
        player.use_on(temp_player)

    other_player = Player("D", 100, Knife(), (6, 7))
    player.use_on(other_player)

    assert player.item.ammunition_behavior.curr_ammo == 0
    assert other_player.hp == 100

    player.reload()
    player.use_on(other_player)

    assert player.item.ammunition_behavior.curr_ammo == player.item.ammunition_behavior.max_ammo - 1
    assert other_player.hp != 100
