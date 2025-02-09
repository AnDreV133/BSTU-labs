import numpy as np
import scipy.integrate as si
import matplotlib.pyplot as plt


def func(y, t):
    C1 = 1e-5

    a = 1

    L0 = 5e-2
    L2 = 5e-2

    R1 = 40
    R2 = 200

    E = 10

    m1 = 5
    m2 = 7
    m3 = 1
    k1 = 1e4
    k2 = 5e3
    k3 = 15e3

    I0 = E / R2

    Q1, Q2, I1, I2, vx1, x1, vx2, x2, vx3, x3 = y

    L1 = L0 - a * abs(x3)
    dL1 = a if x3 < 0 else -a

    return [
        I1,
        I2,
        (E - Q1 / C1 - I1 * R1 + I2 * R1 - dL1 * vx3 * (I0 + I1)) / L1,
        (- I2 * R1 + I1 * R1 - I2 * R2) / L2,
        (-k1 * x1 - k2 * (x1 - x2) - k3 * (x1 - x3)) / m1,
        vx1,
        (- k2 * (x2 - x1)) / m2,
        vx2,
        (-k3 * (x3 - x1)) / m3,
        vx3
    ]


R2 = 200


def main():
    t = np.arange(0.01, 0.1, 0.00001)
    y0 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0.02]
    solution = si.odeint(func, y0, t)

    plt.plot(t, solution[:, 9], 'g--', label='x(t)')
    # plt.plot(t, solution[:, 7], 'y--', label='x(t)')
    # plt.plot(t, solution[:, 5], 'y--', label='x(t)')
    plt.plot(t, solution[:, 2], 'r', label='I1(t)')
    plt.plot(t, solution[:, 3], 'b', label='I2(t)')
    plt.legend(loc='best')
    plt.xlabel('t')
    plt.grid()
    plt.show()

    plt.plot(t, solution[:, 3] * R2, 'b--', label='U(t)')
    plt.legend(loc='best')
    plt.xlabel('t')
    plt.grid()
    plt.show()


if __name__ == '__main__':
    main()

# import numpy as np
# import scipy.integrate as si
# import matplotlib.pyplot as plt
#
#
# def func(y, t):
#     C1 = 1e-5
#
#     a = 1
#
#     L0 = 5e-2
#     L2 = 5e-2
#
#     R1 = 40
#     R2 = 200
#
#     E = 10
#
#     m1 = 5
#     m2 = 7
#     m3 = 1
#     k1 = 1e4
#     k2 = 5e3
#     k3 = 15e3
#
#     Q1, Q2, I1, I2, vx, x = y
#
#     L1 = L0 - a * abs(x)
#     dL1 = a if x < 0 else -a
#
#     I0 = E / R2
#
#     return [
#         I1,
#         I2,
#         (E - Q1 / C1 - I1 * R1 + I2 * R1 - dL1 * vx * (I0 + I1)) / L1,
#         (-I2 * R1 + I1 * R1 - I2 * R2) / L2,
#         (-k1 * x - k2 * x - k3 * x) / m2,
#         vx
#     ]
#
#
# R2 = 200
#
#
# def main():
#     t = np.arange(0, 0.2, 0.00001)
#     y0 = [0, 0, 0, 0, 0, 0.5]
#     solution = si.odeint(func, y0, t)
#
#     plt.plot(t, solution[:, 5], 'g', label='x(t)')
#     plt.plot(t, solution[:, 2], 'r', label='I1(t)')
#     plt.plot(t, solution[:, 3], 'b', label='I2(t)')
#     plt.legend(loc='best')
#     plt.xlabel('t')
#     plt.grid()
#     plt.show()
#
#     plt.plot(t, solution[:, 3] * R2, 'b--', label='U(t)')
#     plt.legend(loc='best')
#     plt.xlabel('t')
#     plt.grid()
#     plt.show()
#
#
# if __name__ == '__main__':
#     main()
