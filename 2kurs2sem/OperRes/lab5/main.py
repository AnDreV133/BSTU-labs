# import numpy as np
#
#
# def make_new_table(m, n, T, Basic, method):
#     count = 1
#     while not check_positive(m, T):
#         min_val = float('inf')
#         mini = 0
#         for i in range(m):
#             if T[i][0] < min_val:
#                 min_val = T[i][0]
#                 mini = i
#
#         min_val = float('inf')
#         minj = 99
#         for i in range(1, n + 1):
#             if T[mini][0] < 0 and T[mini][i] < 0 and T[mini][0] / T[mini][i] < min_val:
#                 min_val = T[mini][0] / T[mini][i]
#                 minj = i
#
#         if minj == 99:
#             if method == 1:
#                 print(f"\nСимплекс-таблица №{count}:\n")
#             else:
#                 print(f"\nОбобщенная симплекс-таблица №{count}:\n")
#             write_table(m, n + 1, T, Basic, 99, 99)
#             print("Задача является недопустимой и решения не имеет")
#             return
#         else:
#             if method == 1:
#                 print(f"\nСимплекс-таблица №{count}:\n")
#             else:
#                 print(f"\nОбобщенная симплекс-таблица №{count}:\n")
#             write_table(m, n + 1, T, Basic, mini, minj)
#             count += 1
#
#             Basic[mini] = minj - 1
#
#             t = T[mini][minj]
#             for i in range(n + 1):
#                 T[mini][i] = T[mini][i] / t
#
#             for i in reversed(range(mini)):
#                 k = T[i][minj]
#                 for j in range(n + 1):
#                     T[i][j] -= T[mini][j] * k
#
#             for i in range(mini + 1, m + 1):
#                 k = T[i][minj]
#                 for j in range(n + 1):
#                     T[i][j] -= T[mini][j] * k
#
#     if method == 1:
#         print(f"\nСимплекс-таблица №{count}:\n")
#     else:
#         print(f"\nОбобщенная симплекс-таблица №{count}:\n")
#     write_table(m, n + 1, T, Basic, 99, 99)
#
#     print(f"Zmax = {T[m][0]}")
#     print("Точка максимума = (", end=' ')
#     for i in range(n):
#         flag = 1
#         for j in range(m):
#             if i == Basic[j] and i < n - m:
#                 print(f"{T[j][0]}", end=' ')
#                 flag = 0
#         if flag and i < n - m:
#             print("0", end=' ')
#     print(")\n")
#
#     print("Точка минимума = (", end=' ')
#     for i in range(n - m + 1, n + 1):
#         print(f"{T[m][i]}", end=' ')
#     print(")\n")
#
#
# def write_table(m, n, T, Basic, a, b):
#     print("б.п\tс.ч\t", end='')
#     for i in range(n - 1):
#         print(f"x{i + 1}\t", end='')
#     print()
#
#     for i in range(m):
#         print(f"x{int(Basic[i] + 1)}\t", end='')
#         for j in range(n):
#             if i == a and j == b:
#                 print(f"[{T[i][j]}]\t ", end='')
#             else:
#                 print(f"{T[i][j]}\t", end='')
#         print()
#
#     print("Z\t", end='')
#     for j in range(n):
#         print(f"{T[m][j]}\t", end='')
#     print()
#
#
# def make_table(m, n, Z, System):
#     T = np.zeros((m+1, n+m+1))
#
#     for i in range(m):
#         T[i][0] = System[i][n]
#     T[m][0] = 0
#
#     for i in range(m):
#         for j in range(1, n + 1):
#             T[i][j] = System[i][j - 1]
#
#     for i in range(1, n + 1):
#         if Z[i - 1]:
#             T[m][i] = -Z[i - 1]
#         else:
#             T[m][i] = 0
#
#     j = n + 1
#     for i in range(m):
#         T[i][j] = 1.0
#         j += 1
#
#     return T
#
#
# def find_basic(m, n, System):
#     Basic = np.zeros(m)
#     t = 0
#
#     for j in range(1, n):
#         k = 0
#         for i in range(m):
#             if System[i][j]:
#                 k += 1
#
#         if k == 1 and t < m:
#             Basic[t] = j - 1
#             t += 1
#
#     if t != m:
#         print("Недостаточно базисных переменных, приведите систему к базисному виду и повторите попытку.")
#         return None
#     return Basic
#
#
# def check_positive(m, T):
#     for i in range(m):
#         if T[i][0] < 0:
#             return 0
#     return 1


import numpy as np
import scipy as sp
import scipy.optimize as opt

Nfeval = 1

def callbackF(Xi):
    global Nfeval
    print ('{0:4d}   {1: 3.6f}   {2: 3.6f}   {3: 3.6f}'.format(Nfeval, Xi[0], Xi[1], Xi[2]))
    Nfeval += 1

if __name__ == "__main__":
    # System = np.array([[-4, 1, -4, 1, 16],
    #                    [2, 3, 4, 2, 32],
    #                    [3, 5, 2, -3, 22]])
    A = np.array([[-4, 2, 3],
                  [1, 3, 5],
                  [-4, 4, 2],
                  [1, 2, -3]])
    b = np.array([-2, 7, 1, 4])
    c = np.array([-16, -32, -22])

    opt.linprog(A_ub=A, b_ub=b, c=c, method="revised simplex", callback=callbackF)
