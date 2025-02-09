# from scipy.integrate import odeint
# from math import cos, sqrt
#
#
# def Solve(m, M, D, t):
#     E = [D[0] * sqrt(m),
#          D[1] * sqrt(m)]
#     V = [[M[0] + E[0], M[1]],
#          [M[0], M[1] + E[1]],
#          [M[0] + E[0], M[1] + E[1]],
#          [M[0] - E[0], M[1] - E[1]]]
#     Mr = [integrate(V[0], t),
#           integrate(V[1], t),
#           integrate(V[2], t),
#           integrate(V[3], t)]
#     ret = [[GetMk(col(Mr, 0), m),
#             GetMk(col(Mr, 1), m)],
#            [GetDk(col(Mr, 0), m),
#             GetDk(col(Mr, 1), m)]]
#     return ret
#
#
# def col(arr, i):
#     return [x[i] for x in arr]
#
#
# def GetMk(X, m):
#     return 1 / m * (X[0] + X[1]) + (X[3] - X[2]) / (2 * m)
#
#
# def GetDk(X, m):
#     return 1 / m * (X[0] ** 2 + X[1] ** 2) + (X[3] ** 2 - X[2] ** 2) / (2 * m) - GetMk(X, m) ** 2
#
#
# def integrate(X, t):
#     [_, Xr] = odeint(GetSystem, X, [0, t])
#     return Xr
#
#
# def GetSystem(X, t):
#     return [FirstEquation(X, t), SecondEquation(X, t)]
#
#
# def FirstEquation(X, t):
#     return X[1] + X[0] ** 2 - t
#
#
# def SecondEquation(X, t):
#     return cos(X[0] * X[1])
#
#
# res = Solve(2, [0.5, 0.], [1, 1], 28)
# for i in range(2):
#     str_m = 'm' + str(i + 1) + 'k'
#     print(str_m)
#     print(res[0][i])
#
# for i in range(2):
#     str_d = 'd' + str(i + 1) + 'k'
#     print(str_d)
#     print(res[1][i])

print(-5//2)