import math

import util

Nv = 14  # номер варинта
Rзд = (318, 300)

M = (30 + Nv) * 1000  # кг бензина
P = 1.12 + 0.1 * Nv  # тысяч человек / км^2

print("Масса бензина:", M, "тонн")
print("Кол-во людей:", P, "тыс.чел/км^2")

R1 = 1.75 * M ** (1 / 3)

Rош = 1.7 * R1

dRф = 1300 * (R1 / Rош) + 50

print("Избыточное давление на границе огненного шара:", dRф)


def Rl(dPф):
    return 1.34 * R1 * ((233 / dPф + 1) ** 2 - 1) ** (1 / 3)


damage_degree_pressure = (100, 60, 40, 10)
damage_degree_radius = []

for Pdamage in damage_degree_pressure:
    radius = Rl(Pdamage)

    damage_degree_radius.append(radius)

    print("При", Pdamage, "получаем", radius)
    for r in Rзд:
        if r < radius:
            print("Для здания на", r, "м")
            # кирпичное здание по малому
            if Pdamage < 8:
                print("Слабое разрушение")
            elif Pdamage < 12:
                print("Среднее разрушение")
            elif Pdamage < 20:
                print("Сильное разрушение")
            else:
                print("Уничтожено")

print()

Q0 = 2000


def F(R):
    return Rош ** 2 * R / ((Rош ** 2 + R ** 2) ** (3 / 2))


def T(R):
    return 1 - 0.058 * math.log(R)


def I(R):
    return Q0 * F(R) * T(R)


IR1 = I(R1)
print("Интесивность теплового излучения для R1:", IR1)

tсв = util.avg((0.45, 0.85)) * M ** (1 / 3)
print("T(R1) =", T(R1))
print("F(R1) =", F(R1))
print("tсв =", tсв)


def Ut(R):
    return I(R) * tсв


UtR1 = IR1 * tсв
print("Тепловой импульс для R1, кДж/м2", UtR1)

P = P / 1000  # перевод в СИ

for radius in damage_degree_radius:
    mUt = Ut(radius)
    print("Радиус", radius, "тепловой импульс", mUt)

    print("Тёмные доски", "не" if mUt < 380 else "\b", "загорятся")

    Sвзрыва = math.pi * radius ** 2
    потери = P * Sвзрыва

    if mUt > 600:
        print("Потери =", потери, "чел")
    else:
        print("Получены травмы")
