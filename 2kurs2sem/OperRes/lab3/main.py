import numpy as np


def simplex_method(A, b, c):
    # Инициализация симплекс-таблицы
    num_vars = len(c)
    num_constraints = len(b)
    table = np.zeros((num_constraints + 1, num_vars
                      + num_constraints + 1))
    # Коэффициенты при переменных в ограничениях
    table[:-1, :num_vars] = A
    # Единичная матрица для базисных переменных
    table[:-1, num_vars:num_vars + num_constraints] \
        = np.eye(num_constraints)
    # Свободные члены ограничений
    table[:-1, -1] = b
    # Коэффициенты целевой функции
    table[-1, :num_vars] = -c
    # Индексы базисных переменных
    basis = list(range(num_vars, num_vars + num_constraints))
    # Выбор ведущего столбца (с наибольшим
    # отрицательным коэффициентом в строке целевой функции)
    while True:
        j0 = np.argmin(table[-1, :-1])
        # Если все коэффициенты неотрицательны,
        # то текущее решение оптимально
        if table[-1, j0] >= 0:
            break
        # Выбор ведущей строки (с минимальным положительным
        # отношением свободного члена к коэффициенту в ведущем столбце)
        i0 = None
        min_ratio = None
        for i in range(num_constraints):
            if table[i, j0] > 0:
                ratio = table[i, -1] / table[i, j0]
                if min_ratio is None or ratio < min_ratio:
                    i0 = i
                    min_ratio = ratio
        # Пересчет таблицы (ведущий элемент приводится к единице,
        # остальные элементы ведущего столбца - к нулю)
        pivot = table[i0, j0]
        table[i0, :] /= pivot
        for i in range(num_constraints + 1):
            if i != i0:
                table[i, :] -= table[i, j0] * table[i0, :]
        # Обновление базиса (замена ведущей переменной в базисе)
        basis[i0] = j0
    # Возвращение результата (оптимальное решение)
    x = np.zeros(num_vars + num_constraints)
    x[basis] = table[:-1, -1]

    return x[:num_vars]


def artificial_basis(A, b, c):
    # Делаем копии матриц, векторов
    sub_A = A[:]
    sub_b = b[:]
    sub_c = c[:]
    # Добавляем искусственные базисы
    for i in range(len(A)):
        if not any(map(lambda x: np.isclose(x, 1), A[:, i])) \
                or not np.isclose(sum(map(abs, A[:, i])), 1):
            # Добавляем искуственные базисы в таблицу
            buf = np.zeros([len(A), 1], dtype=float)
            buf[i] = [1.]
            np.append(sub_A, buf,
                      axis=1)  # баг ------------------------------------------------------------------------------------
            # Добавляем искуственные базисы в целевую функцию
            np.append(sub_c, -1.)
    # Применяем Симплекс-метод
    return simplex_method(sub_A, sub_b, sub_c)


def large_fine(A, b, c):
    # Делаем копии матриц, векторов
    sub_A = A[:]
    sub_b = b[:]
    sub_c = c[:]
    # Добавляем искусственные базисы
    for i in range(len(A)):
        if not any(map(lambda x: np.isclose(x, 1), A[:, i])) \
                or not np.isclose(sum(map(abs, A[:, i])), 1):
            # Добавляем искуственные базисы в таблицу
            buf = np.zeros([len(A), 1], dtype=float)
            buf[i] = [1.]
            np.append(sub_A, buf, axis=1)
            # Добавляем искуственные базисы в целевую функцию
            # с большим числом в константе
            np.append(sub_c, np.float64)
    # Применяем Симплекс-метод
    return simplex_method(sub_A, sub_b, sub_c)


# Пример задачи
A = np.array([[4, 5, 1, 2, -3],
              [2, -1, 4, 1, 1],
              [-3, 2, 6, 4, -8]], dtype=float)
b = np.array([25, 28, 30], dtype=float)
c = np.array([5, -4, 2, -1, 6], dtype=float)
#
# result = artificial_basis(A, b, c)
# print("Оптимальное решение:", result)
#
# result = large_fine(A, b, c)
# print("Оптимальное решение:", result)


import numpy as np
from scipy.optimize import linprog


def artificial_basis_method(c, A, b):
    num_vars = len(c)  # Количество переменных в исходной задаче
    num_constraints = len(b)  # Количество ограничений в исходной задаче
    # Создаем искусственные переменные
    c_artificial = np.concatenate([np.zeros(num_vars),
                                   np.ones(num_constraints)])  # Целевая функция для искусственной задачи
    A_artificial = np.hstack([A, np.eye(num_constraints)])  # Матрица ограничений для искусственной задачи
    # Решаем искусственную задачу
    res = linprog(c=c_artificial, A_ub=A_artificial, b_ub=b, method='highs')
    # Проверяем, есть ли искусственные переменные в базисе
    if any(res.x[num_vars:] != 0):
        raise ValueError("Нет допустимого начального решения")
    # Удаляем искусственные переменные из базиса
    A = A_artificial[:, :num_vars]
    b = b - A_artificial[:, num_vars:].dot(res.x[num_vars:])
    # Решаем исходную задачу
    res = linprog(c=c, A_ub=A, b_ub=b, method='highs')
    return res.x[:num_vars]  # Возвращаем решение исходной задачи


# Пример использования
# c = np.array([-1, -2])  # Коэффициенты целевой функции
# A = np.array([[1, 1], [3, 2]])  # Матрица ограничений
# b = np.array([6, 12])  # Вектор правых частей ограничений
x = artificial_basis_method(c, A, b)  # Вызов функции для решения задачи
print("Решение:", x)  # Вывод решения

import numpy as np
from scipy.optimize import minimize


def big_penalty_method(c, A, b, M=1000):
    num_vars = len(c)  # Количество переменных в исходной задаче
    num_constraints = len(b)  # Количество ограничений в исходной задаче

    # Функция штрафа
    def penalty(x):
        # Штраф за нарушение ограничений, умноженный на большое число M
        return np.sum(np.maximum(0, A.dot(x) - b)) * M

    # Целевая функция с штрафом
    def objective(x):
        # Целевая функция исходной задачи плюс штраф
        return c.dot(x) + penalty(x)

    # Начальное приближение
    x0 = np.zeros(num_vars)
    # Решаем задачу минимизации
    res = minimize(objective, x0)
    return res.x  # Возвращаем решение исходной задачи


# # Коэффициенты целевой функции
# c = np.array([5, -3, 2, -1, 6])
# # Матрица ограничений
# A = np.array([
#     [4, 5, 1, 2, -3],
#     [1, -2, 0, 1, 1],
#     [-3, 2, 6, 4, 8]
# ])
# # Вектор правых частей ограничений
# b = np.array([20, 28, 44])
# Вызов функции для решения задачи
x = big_penalty_method(c, A, b)
print("Решение:", x)
