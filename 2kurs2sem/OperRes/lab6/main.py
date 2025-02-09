import numpy as np
from scipy.optimize import linprog


def get_saddle_point(matrix_game):
    # Проверка на несмешанность стратегий
    max_among_min = np.max([np.min(x) for x in matrix_game])
    index_max_among_min = np.argmax([np.min(x) for x in matrix_game])
    min_among_max = np.min([np.max(x) for x in matrix_game.transpose()])

    if max_among_min == min_among_max:
        return max_among_min, np.array(
            [0.] * index_max_among_min + [1.] + [0.] * (matrix_game.shape[0] - index_max_among_min - 1)
        )

    return None


def get_saddles_point_by_dual_simplex_method(matrix_game):
    # Проверка на несмешанность стратегий
    res = get_saddle_point(matrix_game)
    if not res is None:
        return res[0], res[1]

    # Решение игры в смешанных стратегиях
    (n, m) = np.shape(matrix_game)
    # Описание неравенств
    A_ub = -np.transpose(matrix_game)
    A_ub = np.append(A_ub, np.ones((m, 1)), axis=1)
    b_ub = np.zeros(m)
    # р1 + р2 + ... + рn = 1
    A_eq = np.ones((1, n + 1))
    A_eq[0][n] = 0
    b_eq = np.ones(1)
    # целевая максимизирующая функция
    c = np.zeros(n + 1)
    c[n] = -1
    # передача сформированных данных
    res = linprog(c, A_ub=A_ub, b_ub=b_ub, A_eq=A_eq, b_eq=b_eq, bounds=(0, None),
                  method="highs")

    return res.fun, res.x[:-1]


def output_saddle_point(matrix_game):
    print("Стратегии игроков:")
    print(gm)
    res_fun, res_x = get_saddles_point_by_dual_simplex_method(matrix_game)
    print("Стратегия:", res_x)
    print("Седловая точка:", res_fun)


gm = np.array([
    [8, 4, 5, 9],
    [7, 10, 6, 7],
    [9, 3, 5, 6]
])

output_saddle_point(gm)
