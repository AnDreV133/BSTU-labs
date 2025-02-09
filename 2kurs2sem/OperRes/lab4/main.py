import numpy as np


def northwest_corner_method(u, v):
    if u.sum() != v.sum():
        raise Exception("Задача несбалансирована, иди балансируй")

    u_counter = u.copy()
    v_counter = v.copy()

    num_u = len(u)
    num_v = len(v)

    allocation = np.zeros((num_u, num_v))

    current_u = 0
    current_v = 0
    while current_u < num_u and current_v < num_v:
        # Выбираем минимум из доступного запаса и требования
        quantity = min(u[current_u], v[current_v])

        # Выделяем запас по правилу северо-западного угла
        allocation[current_u, current_v] = quantity

        u_counter[current_u] -= quantity
        v_counter[current_v] -= quantity

        if u_counter[current_u] <= 0 and current_u < num_u:
            current_u += 1
        if v_counter[current_v] <= 0 and current_v < num_v:
            current_v += 1

    return allocation


def costs_grid_free_values(costs, u_potentials, v_potentials, allocation):
    num_u = len(u_potentials)
    num_v = len(v_potentials)

    costs_grid = np.zeros((num_u, num_v))
    for i in range(num_u):
        for j in range(num_v):
            if allocation[i, j] == 0:
                costs_grid[i, j] = costs[i, j] - (u_potentials[i] + v_potentials[j])
            else:
                costs_grid[i, j] = allocation[i, j]

    return costs_grid


def find_potentials(cost_matrix, allocation):
    m, n = allocation.shape
    u = np.full(m, float("-inf"))
    v = np.full(n, float("-inf"))

    # Выбираем неназначенные клетки
    non_zero_indices = np.argwhere(allocation > 0)

    # Итеративно находим потенциалы
    while len(non_zero_indices) > 0:
        # Обновляем неназначенные клетки
        if u[non_zero_indices[0][0]] == float("-inf"):
            u[non_zero_indices[0][0]] = 0
        elif v[non_zero_indices[0][1]] == float("-inf"):
            v[non_zero_indices[0][1]] = 0

        for i, j in non_zero_indices:
            if u[i] == float("-inf") and v[j] == float("-inf"):
                continue
            elif u[i] == float("-inf"):
                u[i] = cost_matrix[i][j] - v[j]
            elif v[j] == float("-inf"):
                v[j] = cost_matrix[i][j] - u[i]

        non_zero_indices = non_zero_indices[1:]

    for index in range(m):
        if u[index] == float("-inf"):
            u[index] = 0

    for index in range(n):
        if v[index] == float("-inf"):
            v[index] = 0

    return u, v


# def calculate_potentials(costs, allocation):
#     m, n = allocation.shape
#     u = np.full(m, float("-inf"))
#     v = np.full(n, float("-inf"))
#
#     u[0] = 0
#     max_iterations = m * n  # Максимальное число итераций
#
#     def calculate_potentials_horizontally(i):
#         nonlocal max_iterations, costs, allocation
#         max_iterations -= 1
#
#         if max_iterations == 0:
#             raise Exception("Зацикливание при вычислении потенциалов")
#         #
#         # if u[i] is None:
#         #     raise Exception(f"Ошибка получения потенциала u[{i}]")
#
#         for j in range(n):
#             if allocation[i][j] == 0:
#                 continue
#             if v[j] != float("-inf"):
#                 continue
#             else:
#                 v[j] = costs[i][j] - u[i]
#                 calculate_potentials_vertically(j)
#
#     def calculate_potentials_vertically(j):
#         # if v[j] is None:
#         #     raise Exception(f"Ошибка получения потенциала v[{j}]")
#
#         nonlocal costs, allocation
#         for i in range(m):
#             if allocation[i][j] == 0:
#                 continue
#             if u[i] != float("-inf"):
#                 continue
#             else:
#                 u[i] = costs[i][j] - v[j]
#                 calculate_potentials_horizontally(i)
#
#     calculate_potentials_horizontally(0)  # Начало рекурсии
#
#     # for i in range(m):
#     #     if u[i] is None:
#     #         print(f"Не удалось вычислить потенциал u[{i}]")
#     #         return None
#     #
#     # for j in range(n):
#     #     if v[j] is None:
#     #         print(f"Не удалось вычислить потенциал v[{j}]")
#     #         return None
#
#     return u, v


def correct_solve_by_cycle(costs_grid, allocation):
    allocation = allocation.copy()
    m, n = allocation.shape

    i_begin = 0
    j_begin = 0
    for i in range(m):
        if np.min(costs_grid[i]) < costs_grid[i_begin][j_begin]:
            j_begin = np.argmin(costs_grid[i])
            i_begin = i

    vertical_allocation = [j for j in range(n) if allocation[i_begin, j] > 0]
    horizontal_allocation = [i for i in range(m) if allocation[i, j_begin] > 0]

    i_end = -1
    j_end = -1
    break_flag = False
    for vert in vertical_allocation:
        for horiz in horizontal_allocation:
            if allocation[horiz][vert] > 0:
                i_end = horiz
                j_end = vert
                break_flag = True
                break

        if break_flag:
            break

    min_subtraction_value = min(allocation[i_begin, j_end], allocation[i_end, j_begin])

    allocation[i_begin][j_end] -= min_subtraction_value
    allocation[i_end][j_begin] -= min_subtraction_value
    allocation[i_begin][j_begin] += min_subtraction_value
    allocation[i_end][j_end] += min_subtraction_value

    return allocation


def find_loop_and_redistribute(costs_grid, allocation):
    allocation = allocation.copy()
    m, n = allocation.shape

    max_iterations = m * n
    cycle_cord = []
    i_begin = 0
    j_begin = 0

    neg_val_indices = np.argwhere(costs_grid < 0)

    def find_loop_horizontally(i0, j0):
        nonlocal max_iterations
        max_iterations -= 1
        if max_iterations == 0:
            raise Exception("Слишком большое число итераций при поиске цикла")

        for j in range(n):
            if j == j0:
                continue
            if allocation[i0][j] == 0:
                continue
            if find_loop_vertically(i0, j):
                cycle_cord.append((i0, j))
                return True
        return False

    def find_loop_vertically(i0, j0):
        # nonlocal i_begin, j_begin
        for i in range(m):
            if i == i_begin and j0 == j_begin:
                cycle_cord.append((i, j0))
                return True
            if i == i0:
                continue
            if allocation[i][j0] == 0:
                continue
            if find_loop_horizontally(i, j0):
                cycle_cord.append((i, j0))
                return True
        return False

    for index in neg_val_indices:
        i_begin = index[0]
        j_begin = index[1]
        if find_loop_horizontally(i_begin, j_begin):
            return cycle_cord

    return None


def redistribution_by_cycle(cycle_cord, allocation):
    theta = None
    sign = "+"
    for indexes in cycle_cord:
        i = indexes[0]
        j = indexes[1]
        if sign == "-":
            volume = allocation[i][j]
            if theta is None:
                theta = volume
            else:
                if volume < theta:
                    theta = volume
            sign = "+"
        else:
            sign = "-"

    if theta is None:
        raise Exception("Не удалось вычислить переменную тета")

    # print("Тета:", theta)
    if theta == 0:
        return False

    sign = "+"
    for indexes in cycle_cord:
        i = indexes[0]
        j = indexes[1]
        if sign == "-":
            allocation[i][j] -= theta
            sign = "+"
        else:
            allocation[i][j] += theta
            sign = "-"

    return True


def main():
    # Пример использования функции
    u = np.array([18, 12, 22, 19])
    v = np.array([14, 11, 17, 15, 14])

    costs = np.array([[9, 21, 22, 14, 10],
                      [30, 34, 42, 23, 26],
                      [8, 17, 30, 27, 9],
                      [11, 20, 24, 7, 25]])

    allocation = northwest_corner_method(u, v)
    print("Начальное распределение:")
    print(allocation)

    while True:
        u_potentials, v_potentials = find_potentials(costs, allocation)
        # u_potentials, v_potentials = calculate_potentials(costs, allocation)
        print("Потенциалы для отправителей:", u_potentials)
        print("Потенциалы для получателей:", v_potentials)

        if np.any(u_potentials < 0) or np.any(v_potentials < 0):
            costs_grid = costs_grid_free_values(costs, u_potentials, v_potentials, allocation)
            print("Сетка стоимостей для свободных клеток:")
            print(costs_grid)

            if np.all(costs_grid >= 0):
                break

            # allocation = correct_solve_by_cycle(costs_grid, allocation)
            # print(allocation)

            cycle_cord = find_loop_and_redistribute(costs_grid, allocation)
            redistribution_by_cycle(cycle_cord, allocation)
            print("Пересчитанное распределение по циклу:")
            print(allocation)
        else:
            break

    print("Итоговая стоимость транспортировки:")
    print(np.sum(allocation * costs))


if __name__ == '__main__':
    main()



