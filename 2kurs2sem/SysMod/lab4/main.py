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
    return + k1 * x


def equationOY(x, y):
    return + m * g \
        + k2 * y \
        + k3 * y


def solve(s, t):  # s1 = x; s3 = y
    dv1dt = -equationOX(s[1], s[3]) / m
    dXdt = s[0]
    dv2dt = -equationOY(s[1], s[3]) / m
    dYdt = s[2]

    return [dv1dt, dXdt, dv2dt, dYdt]


def main():
    t = np.linspace(0, 3, 100)
    s0 = [0, 0, 0, 0]
    s = odeint(solve, s0, t)
    print(t)
    v1 = s[:, 0]
    X = s[:, 1]
    v2 = s[:, 2]
    Y = s[:, 3]

    plt.xlim(0, 3)
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
