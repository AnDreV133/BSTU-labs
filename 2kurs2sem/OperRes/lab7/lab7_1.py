from scipy.optimize import linprog
import numpy as np
import math as m

A = np.array([[7, 5, 1, 0, 0],
              [4, -6, 0, 3, 0],
              [-3, 4, 0, 0, 1]], dtype=float)
b = np.array([28, 14, 6], dtype=float)
c = np.array([3, 1, 0, 0, - 1], dtype=float)


def method_clipping(A, b, c, max_iter=10):
    A_copy = np.copy(A)
    b_copy = np.copy(b)
    c_copy = np.copy(c)
    res = None
    i_iter = 0
    while i_iter < max_iter:
        # решение симплекс методом
        res = linprog(-c_copy, A_ub=A_copy, b_ub=b_copy, bounds=[(0, None)] * len(c), method='highs')
        if not res.success:
            break

            # проверка свободных членов
        slack = res.get("slack")
        if all(is_int(slack)):
            break

        # находим максимальный индекс дробной части
        index_max_fractial = 0
        for i in range(len(b)):
            if slack[i] % 1 > slack[index_max_fractial] % 1:
                index_max_fractial = i

        # вычисление сечения Гомори
        temp = []
        for k in range(len(c)):
            temp.append(A_copy[index_max_fractial, k])

        A_copy = np.vstack((A_copy, temp))
        b_copy = np.append(b_copy, b_copy[index_max_fractial] % 1)
        A_copy = np.hstack((A_copy, np.array_split([0] * (len(b_copy) - 1) + [1], len(b_copy))))
        c_copy = np.append(c_copy, 0)

        i_iter += 1 # todo invalid input for linprog

    return res


def is_int(n, epsilon=0.001):
    return abs(n % 1) <= epsilon


if __name__ == '__main__':
    res = method_clipping(A, b, c)
    print(res)
