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


def main():
    # Пример использования
    A = np.array([[1, -4, 0, -5, 0, -3],
                  [0, 7, 0, 5, 1, 4],
                  [-0, 3, 1, -5, 0, -4]])
    b = np.array([9, 26, 10])
    c = np.array([0, 6, 0, 8, 0, 3])
    x = simplex_method(A, b, c)

    buf = ""
    for i in range(len(c)):
        if c[i] != 0:
            buf += str(c[i]) + "x_" + str(i + 1) + " + "
    buf += "\b\b-> max"

    print("Целевая функция: ", buf)
    print("Решение: ", x, "; Fmax= ", sum(c * x), sep="")


if __name__ == "__main__":
    main()
