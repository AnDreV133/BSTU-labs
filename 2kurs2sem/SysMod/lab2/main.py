from math import *
import matplotlib.pyplot as plt
from scipy.optimize import fsolve
from scipy.integrate import odeint
import warnings
import numpy as np

k1 = 20000.
k2 = 10000.
k3 = 40000.
n1 = 1.
n2 = 0.5
n3 = 0.7
m = 10.
dif = 0.001
g = 9.8

F = 0


def equationOX(x, y):
    c1 = np.sqrt(pow(n1 + x, 2) + y * y)
    c2 = np.sqrt(pow(n2 - y, 2) + x * x)
    c3 = np.sqrt(pow(n3 + y, 2) + x * x)
    return - k1 * (c1 - n1) * (n1 + x) / c1 \
           - k2 * (c2 - n2) * x / c2 \
           - k3 * (c3 - n3) * x / c3


def equationOY(x, y):
    c1 = np.sqrt(pow(n1 + x, 2) + y * y)
    c2 = np.sqrt(pow(n2 - y, 2) + x * x)
    c3 = np.sqrt(pow(n3 + y, 2) + x * x)
    return + m * g \
           - k1 * (c1 - n1) * y / c1 \
           + k2 * (c2 - n2) * (n2 - y) / c2 \
           - k3 * (c3 - n3) * (n3 + y) / c3


def solve(s, t):  # s1 = x; s3 = y
    dv1dt = equationOX(s[1], s[3]) / m
    dXdt = s[0]
    dv2dt = equationOY(s[1], s[3]) / m
    dYdt = s[2]

    return [dv1dt, dXdt, dv2dt, dYdt]


def main():
    t = np.linspace(0, 1, 1000)
    s0 = [0, 0, 0, 0]
    s = odeint(solve, s0, t)
    print(t)
    v1 = s[:, 0]
    X = s[:, 1]
    v2 = s[:, 2]
    Y = s[:, 3]

    plt.xlim(0, 1)
    # plt.plot(t, v1, 'r--', label="v1(t)")
    plt.plot(t, X, 'b-', label="X(t)")
    # plt.plot(t, v2, 'y--', label="v2(t)")
    plt.plot(t, Y, 'g--', label="Y(t)")

    plt.xlabel("t")
    plt.ylabel("v1(t), X(t), v2(t), Y(t)")
    plt.ylabel("X(t), Y(t)")

    plt.legend()
    plt.grid()
    plt.show()


if __name__ == '__main__':
    main()
