import random

i, j, k = 1, 8, 7

p = [
    i / 2 * (i + j + k),
    (j + k) / 2 * (i + j + k),
    (i + k) / 2 * (i + j + k),
    j / 2 * (i + j + k)
]
p = list(map(lambda x: x / sum(p), p))
print("p=", p)

count = [0] * 4
num_exp = 10000
for _ in range(num_exp):
    rand = random.random()
    if rand < p[0]:
        count[0] += 1
    elif rand < p[0] + p[1]:
        count[1] += 1
    elif rand < p[0] + p[1] + p[2]:
        count[2] += 1
    else:
        count[3] += 1

count = list(map(lambda x: x / num_exp, count))
print("count=", count)

alpha = 0.05
if all(map(lambda x: abs(x[0] - x[1]) < alpha, zip(count, p))):
    print("Соответсвует распределению")
else:
    print("Не соответствует распределению")
