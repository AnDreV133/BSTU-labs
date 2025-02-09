import pygame
from random import choice

WIDTH, HEIGHT = 750, 750
TILE = 50
INDENT = 10

COLS = 8
ROWS = 8

CELL_COLOR = pygame.Color('white')
CELL_START_COLOR = pygame.Color('green')
CELL_FINISH_COLOR = pygame.Color('blue')
WALL_COLOR = pygame.Color('black')
PLAYER_COLOR = pygame.Color('red')

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Maze example")
clock = pygame.time.Clock()


class Cell:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.walls = {'top': True, 'right': True, 'bottom': True, 'left': True}
        self.visited = False

    def draw(self):
        x_2 = 2 * self.x
        y_2 = 2 * self.y

        if self.visited:
            self.draw_cell(x_2, y_2)

        if not self.walls['top']:
            self.draw_cell(x_2, y_2 - 1)
        if not self.walls['right']:
            self.draw_cell(x_2 + 1, y_2)
        if not self.walls['bottom']:
            self.draw_cell(x_2, y_2 + 1)
        if not self.walls['left']:
            self.draw_cell(x_2 - 1, y_2)

    @staticmethod
    def draw_cell(x, y, color=CELL_COLOR):
        pygame.draw.rect(screen, color, (x * TILE, y * TILE, TILE, TILE))

    @staticmethod
    def check_cell(x, y):
        if x < 0 or x > COLS - 1 or y < 0 or y > ROWS - 1:
            return False
        return grid_cell[x + y * COLS]

    def check_neighbours(self):
        neighbours = []

        top = self.check_cell(self.x, self.y - 1)
        right = self.check_cell(self.x + 1, self.y)
        bottom = self.check_cell(self.x, self.y + 1)
        left = self.check_cell(self.x - 1, self.y)

        if top and not top.visited:
            neighbours.append(top)
        if right and not right.visited:
            neighbours.append(right)
        if bottom and not bottom.visited:
            neighbours.append(bottom)
        if left and not left.visited:
            neighbours.append(left)

        return choice(neighbours) if neighbours else False


class Player:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def move_up(self, _cell):
        if not _cell.walls['top'] and self.y > 0:
            self.y -= 1
            return True
        return False

    def move_right(self, _cell):
        if not _cell.walls['right'] and self.x < 2*COLS - 1:
            self.x += 1
            return True
        return False

    def move_down(self, _cell):
        if not _cell.walls['bottom'] and self.y < 2*ROWS - 1:
            self.y += 1
            return True
        return False

    def move_left(self, _cell):
        if not _cell.walls['left'] and self.x > 0:
            self.x -= 1
            return True
        return False

    @staticmethod
    def draw_player(x, y):
        pygame.draw.rect(screen, PLAYER_COLOR, (
            x * TILE + INDENT,  # todo
            y * TILE + INDENT,
            TILE - INDENT * INDENT,
            TILE - INDENT * INDENT
        ))


def remove_walls(current_cell, next_cell):
    dx = current_cell.x - next_cell.x
    dy = current_cell.y - next_cell.y

    if dx == 1:
        current_cell.walls['left'] = False
        next_cell.walls['right'] = False
    if dx == -1:
        current_cell.walls['right'] = False
        next_cell.walls['left'] = False
    if dy == 1:
        current_cell.walls['top'] = False
        next_cell.walls['bottom'] = False
    if dy == -1:
        current_cell.walls['bottom'] = False
        next_cell.walls['top'] = False


def check_wall(grid_cell, x, y):
    if x % 2 == 0 and y % 2 == 0:
        return False
    if x % 2 == 1 and y % 2 == 1:
        return True

    if x % 2 == 0:
        grid_x = x // 2
        grid_y = (y - 1) // 2
        return grid_cell[grid_x + grid_y * COLS].walls['bottom']
    else:
        grid_x = (x - 1) // 2
        grid_y = y // 2
        return grid_cell[grid_x + grid_y * COLS].walls['right']


grid_cell = [Cell(x, y) for y in range(ROWS) for x in range(COLS)]

current_cell = grid_cell[0]
current_cell.visited = True

player = Player(0, 0)

stack = []

while True:
    screen.fill(WALL_COLOR)

    for cell in grid_cell:
        cell.draw()

    Cell.draw_cell(0, 0, CELL_START_COLOR)
    Cell.draw_cell(2 * (COLS - 1), 2 * (ROWS - 1), CELL_FINISH_COLOR)
    # Player.draw_player(player.x, player.y)

    next_cell = current_cell.check_neighbours()
    if next_cell:
        next_cell.visited = True
        remove_walls(current_cell, next_cell)
        current_cell = next_cell
        stack.append(current_cell)
    elif stack:
        current_cell = stack.pop()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            exit()
        if event.type == pygame.KEYDOWN:
            # Cell.draw_cell(player.x, player.y)
            match event.key:
                case pygame.K_DOWN:
                    player.move_down(grid_cell[player.x + player.y * COLS])
                case pygame.K_UP:
                    player.move_up(grid_cell[player.x + player.y * COLS])
                case pygame.K_LEFT:
                    player.move_left(grid_cell[player.x + player.y * COLS])
                case pygame.K_RIGHT:
                    player.move_right(grid_cell[player.x + player.y * COLS])
    Cell.draw_cell(player.x, player.y, PLAYER_COLOR)
        # Player.draw_player(player.x, player.y)

    pygame.display.flip()
    # clock.tick(30)

# map_cell = [check_wall(grid_cell, x, y) for y in range(ROWS * 2 - 1) for x in range(COLS * 2 - 1)]
# for y in range(ROWS * 2 - 1):
#     for x in range(COLS * 2 - 1):
#         if map_cell[x + y * (COLS * 2 - 1)]:
#             print(" ", end="")
#         else:
#             print("#", end="")
#     print()
