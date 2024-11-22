from math import sqrt


def func(n: float, q: float):
    return \
            n / q ** n \
            + 2 * n / q ** (2 * n) \
            + 4 * n / q ** (4 * n)


S = 0
q = sqrt(3)
for n in range(1, 25):
    S += func(n, q)
    print(f"n = {n}, S = {S}")
