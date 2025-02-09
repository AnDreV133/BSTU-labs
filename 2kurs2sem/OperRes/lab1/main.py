import itertools
from itertools import *


def input_linear_system():
    # Вводим неоднородные уравнения системы линейных уравнений
    equations = []
    num_equations = int(input("Введите количество уравнений: "))

    print("Введите коэффициенты и константы каждого уравнения по очереди:")
    for i in range(num_equations):
        equation = list(map(int, input(f"Уравнение {i + 1}: ").split()))
        equations.append(equation)

    return equations


def float_equal_zero(x):
    return abs(x) < 0.000001;


def float_equal_one(x):
    return 0.999999 < abs(x) < 1.000001;


def output_matrix(m):
    for mi in m:
        print(mi)
    print("\n")


def swap_lines(eqs, r1, r2):
    temp = eqs[r1]
    eqs[r1] = eqs[r2]
    eqs[r2] = temp


def multyply_line(eqs, r, x: float):
    if float_equal_zero(x):
        return

    for i in range(len(eqs[r])):
        eqs[r][i] *= x


def multiply_and_add_to_other_line(eqs, r, oth_r, x: float):
    temp = eqs[r].copy()
    for i in range(len(temp)):
        temp[i] *= x

    for i in range(len(eqs[r])):
        eqs[oth_r][i] += temp[i]


def f(eqs):
    output_matrix(eqs)

    if float_equal_zero(eqs[0][0]):
        swap_lines(eqs, 0, 1)

    multyply_line(eqs, 0, 1. / eqs[0][0])
    multiply_and_add_to_other_line(eqs, 0, 1, -eqs[1][0])
    output_matrix(eqs)

    if float_equal_zero(eqs[1][1]):
        swap_lines(eqs, 1, 2)
        multyply_line(eqs, 0, 1. / eqs[0][0])
        multiply_and_add_to_other_line(eqs, 0, 1, -eqs[1][0])
        output_matrix(eqs)

    multyply_line(eqs, 1, 1. / eqs[1][1])
    multiply_and_add_to_other_line(eqs, 0, 2, -eqs[2][0])
    multiply_and_add_to_other_line(eqs, 1, 2, -eqs[2][1])
    output_matrix(eqs)

    if float_equal_zero(eqs[2][2]):
        swap_lines(eqs, 2, 3)
        multyply_line(eqs, 1, 1. / eqs[1][1])
        multiply_and_add_to_other_line(eqs, 0, 2, -eqs[2][0])
        multiply_and_add_to_other_line(eqs, 1, 2, -eqs[2][1])
        output_matrix(eqs)

    multyply_line(eqs, 1 + 1, 1. / eqs[1 + 1][1 + 1])
    multiply_and_add_to_other_line(eqs, 0, 2 + 1, -eqs[2 + 1][0])
    multiply_and_add_to_other_line(eqs, 1, 2 + 1, -eqs[2 + 1][1])
    multiply_and_add_to_other_line(eqs, 2, 2 + 1, -eqs[2 + 1][2])
    output_matrix(eqs)


def f_(eqs):
    i = 0
    while i < len(eqs) - 1:
        if float_equal_zero(eqs[i][i]):
            swap_lines(eqs, i, i + 1)
            i = i - 1 if i != 0 else i
            continue

        multyply_line(eqs, i, 1. / eqs[i][i])
        for j in range(i + 1):
            multiply_and_add_to_other_line(eqs, j, i + 1, -eqs[i + 1][j])

        output_matrix(eqs)
        i += 1


def get_basic_view(eqs):
    res = eqs.copy()
    # Главный цикл для построчного прохода
    for i in range(len(res) - 1):
        # Условие которое запускает цикл для
        # поиска колонки с ненулевым значением
        # по диагонали
        if float_equal_zero(res[i][i]):
            for j in range(len(res)):
                if not not not float_equal_zero(res[j][i]):
                    swap_lines(res, i, j)
        # Приводим к единице на диагонали
        multyply_line(res, i, 1. / res[i][i])
        # Приводим к нулям в текущей колонке
        for j in range(len(res)):
            if i != j:
                multiply_and_add_to_other_line(res, i, j, -res[j][i])

    return res


def get_all_basic_view(eqs):
    # Получаем все перестановки
    orders = combination_with_tail(len(eqs[0]) - 1, len(eqs)-1)

    # Формируем все базисные виды, ассоциируя
    # их с сгенерированными порядками
    res = []
    for order in orders:
        temp = [[] for _ in range(len(eqs))]
        for index_order in order:
            for i in range(len(eqs)):
                temp[i].append(eqs[i][index_order])

        for i in range(len(eqs)):
            temp[i].append(eqs[i][-1])

        res.append(get_basic_view(temp))

    return res, orders


def combination_with_tail(n, k):
    # Формируем комбинации
    combs = itertools.combinations(range(n), k)
    # Приписываем остальную часть
    res = []
    for comb in combs:
        buf = list(comb)
        for i in range(n):
            if not (i in buf):
                buf.append(i)

        res.append(buf)

    return res


def output_basic_view(eqs_basic, order):
    for line in eqs_basic:
        # Производим поиск единицы для
        # определения его в базисе
        index_basis = -1
        for index_const in range(len(line)):
            if float_equal_one(line[index_const]):
                index_basis = index_const
                break
        # Если переменная не наёдена -
        # значит пропускаем итерацию
        if index_basis < 0:
            continue
        # Вывод с опорой на заданный порядок
        res = "x_" + str(order[index_basis] + 1) + " = "
        for index_const in range(index_basis + 1, len(line) - 1):
            if not float_equal_zero(line[order[index_const]]):
                res += "(" + str(-round(line[order[index_const]], 4)) + ")x_" + str(order[index_const] + 1) + " + "
        res += "(" + str(round(line[-1], 4)) + ")"

        print(res)


def output_all_basic_view(eqs_basics, orders):
    for i in range(len(eqs_basics)):
        print(i + 1, "- результат:")
        output_basic_view(eqs_basics[i], orders[i])


def find_max_basis_sum(eqs_basics):
    # Поиск максимальной суммы свободных членов
    max_summa = float('-inf')
    index_res = -1
    summa = 0
    for i in range(len(eqs_basics)):
        for line in eqs_basics[i]:
            summa += line[-1]

        if summa > max_summa:
            max_summa = summa
            index_res = i

        summa = 0

    return max_summa, index_res


def test_one_iter():
    eqs = [
        [6., -2., 2., 0., 5., 6.],
        [1., -2., 3., 4., -1., 2.],
        [4., -7., 2., 1., 3., 0.],
        [2., -4., 6., 8., -2., 4.],
    ]
    print("Получение первого базиса:")
    output_basic_view(get_basic_view(eqs), [0, 1, 2, 3, 4])


def test_shaffle():
    eqs = [
        [6., -2., 2., 0., 5., 6.],
        [1., -2., 3., 4., -1., 2.],
        [4., -7., 2., 1., 3., 0.],
        [2., -4., 6., 8., -2., 4.],
    ]
    print("Получение всех базисов:")
    temp = get_all_basic_view(eqs)
    output_all_basic_view(temp[0], temp[1])


def test_get_plan():
    eqs = [
        [6., -2., 2., 0., 5., 6.],
        [1., -2., 3., 4., -1., 2.],
        [4., -7., 2., 1., 3., 0.],
        [2., -4., 6., 8., -2., 4.],
    ]

    temp = find_max_basis_sum(get_all_basic_view(eqs)[0])
    print(temp[0], "- максимальная сумма на позиции:", temp[1])


# -------------------------------------------------


if __name__ == '__main__':
    # test_one_iter()
    # test_shaffle()
    test_get_plan()
