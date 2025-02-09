import math
import numpy as np

# Инициализация параметров
# k1 = 20000.
k1 = 0.1
k2 = 10000.
k3 = 40000.
n1 = 1.
n2 = 0.5
n3 = 0.7
m = 10.
Power = 0

EPS = 0.00001


def l2(x, y):
    return x ** 2 + y ** 2


def n1_(x, y):
    return math.sqrt((x - n1) ** 2 + y ** 2)


def n2_(x, y):
    return math.sqrt(x ** 2 + (y - n2) ** 2)


def n3_(x, y):
    return math.sqrt(x ** 2 + (y + n3) ** 2)


def cos_alpha(x, y):
    return (n3 ** 2 + n3_(x, y) ** 2 - l2(x, y)) / (2 * n3 * n3_(x, y))


def cos_beta(x, y):
    return (n2 ** 2 + n2_(x, y) ** 2 - l2(x, y)) / (2 * n2 * n2_(x, y))


def cos_gama(x, y):
    return (n1 ** 2 + n1_(x, y) ** 2 - l2(x, y)) / (2 * n1 * n1_(x, y))


def sin_alpha(x, y):
    return math.sqrt(1 - cos_alpha(x, y) ** 2)


def sin_beta(x, y):
    return math.sqrt(1 - cos_beta(x, y) ** 2)


def sin_gama(x, y):
    return math.sqrt(1 - cos_gama(x, y) ** 2)


def delta1(x, y):
    return n1_(x, y) - n1


def delta2(x, y):
    return n2_(x, y) - n2


def delta3(x, y):
    return n3_(x, y) - n3


def func1(x, y):
    return k1 * delta1(x, y) * cos_gama(x, y) + k2 * delta2(x, y) * sin_beta(x, y) \
           + k3 * delta3(x, y) * sin_alpha(x, y)


def func2(x, y):
    return m * 9.8 + k1 * delta1(x, y) * sin_gama(x, y) \
           + k2 * delta2(x, y) * cos_beta(x, y) + k3 * delta3(x, y) * cos_alpha(x, y) - Power


def func1_x_derivative(x, y):
    return (func1(x + EPS, y) - func1(x - EPS, y)) / (2 * EPS)


def func1_y_derivative(x, y):
    return (func1(x, y + EPS) - func1(x, y - EPS)) / (2 * EPS)


def func2_x_derivative(x, y):
    return (func2(x + EPS, y) - func2(x - EPS, y)) / (2 * EPS)


def func2_y_derivative(x, y):
    return (func2(x, y + EPS) - func2(x, y - EPS)) / (2 * EPS)


def next_point(x, y):
    # print(func1_x_derivative(x, y))
    # print(func2_y_derivative(x, y))
    # print(func1_y_derivative(x, y))
    # print(func2_x_derivative(x, y))
    #
    # print(func1(x, y))
    # print(func2(x, y))
    gen_det = func1_x_derivative(x, y) * func2_y_derivative(x, y) - func1_y_derivative(
        x, y) * func2_x_derivative(x, y)
    x_det = (-func1(x, y)) * func2_y_derivative(x, y) \
            - func2_y_derivative(x, y) * (-func2(x, y))
    y_det = func1_x_derivative(x, y) * (-func2(x, y)) \
            - (-func1(x, y)) * func2_x_derivative(x, y)

    return x + x_det / gen_det, y + y_det / gen_det


def newton(x, y, n, eps):
    while n > 0 and abs(func1(x, y) + func2(x, y)) > eps:
        x, y = next_point(x, y)

        n -= 1

    return x, y


def solve():
    global Power
    i = 2.0
    step = 1.
    while i < 10000:
        Power = -i
        x, y = 0, 0
        result = newton(x, y, 1000, 0.0001)

        # print(func1(result[0], result[1]))
        # print(func2(result[0], result[1]))
        cond = abs(func1(result[0], result[1])) + abs(
                func2(result[0], result[1]))
        if cond > 1.:
            i -= step
            step /= 10
            print("peek")
        else:
            print("{:.4f}".format(i), "\t", "{:.2f}".format(result[0]), "\t", "{:.2f}".format(result[1]))

        if step < 0.0000000001:
            break

        i += step


if __name__ == '__main__':
    solve()
