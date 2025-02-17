import pandas as pd


def средняя_производительность():
    res = []
    for i in range(len(data_mbf)):
        loc = float(data_mbf['loc'][i])
        spending = int(data_mbf['spending'][i])
        res.append(loc / spending)

    return sum(res) / len(res)


def средняя_loc():
    res = list(map(float, data_mbf['loc']))

    return sum(res) / len(res)


delimiter_length = 70

data_mbf = pd.read_csv(
    "data-mbf-my.csv",
    sep=';'
)
data_op = pd.read_csv(
    "data-op-my.csv",
    sep=';'
)
произв_ср = средняя_производительность()
loc_ср = средняя_loc()

buf: dict = {
    "loc_waiting": [],
    "prod_i": [],
    "prod_i_an": [],
    "unit_cost": []
}
for i in range(len(data_mbf)):
    proj = str(data_mbf['proj'][i])
    loc = float(data_mbf['loc'][i])
    spending = int(data_mbf['spending'][i])
    cost = float(data_mbf['cost'][i])
    errors = int(data_mbf['errors'][i])
    doc = int(data_mbf['doc'][i])

    print("-" * delimiter_length)

    print("Проект", proj)

    производительность = loc / spending
    print("Производительность:", производительность, "тыс.LOC/чел.-мес")

    качество = errors / loc
    print("Качество:", качество, "ед./тыс.LOC")

    удельная_стоимость = cost / loc
    print("Удельная_стоимость:", удельная_стоимость, "у.е./тыс.LOC")

    документированность = doc / loc
    print("Документированность:", документированность, "страниц/тыс.LOC")

    buf['unit_cost'].append(удельная_стоимость)

for i in range(len(data_op)):
    func = str(data_op['func'][i])
    loc_best = float(data_op['loc_best'][i])
    loc_top = float(data_op['loc_top'][i])
    loc_worst = float(data_op['loc_worst'][i])

    print("-" * delimiter_length)
    print("Функция", func)

    loc_ожидания = (loc_best + loc_worst + 4 * loc_top) / 6
    print("LOC ожидания:", loc_ожидания, "тыс.LOC")

    print("Производительность (подход 1):", произв_ср, "тыс.LOC/чел.-мес")

    произв_i = произв_ср * loc_ср / loc_ожидания
    print("Производительность (подход 2):", произв_i, "тыс.LOC/чел.-мес")

    # произв_i_ан = производительность * loc_ср / loc_ожидания
    # print("Производительность (подход 3):", произв_i_ан, "тыс.LOC/чел.-мес")

    buf['prod_i'].append(произв_i)
    # buf['prod_i_an'].append(произв_i_ан)
    buf['loc_waiting'].append(loc_ожидания)

loc_waiting = buf['loc_waiting']
prod_i = buf['prod_i']
# prod_i_an = buf['prod_i_an']
unit_cost = buf['unit_cost']

print("~" * delimiter_length)

затраты_1 = sum(loc_waiting) / произв_ср
print("Затраты (подход 1):", затраты_1, "чел.−мес")

затраты_2 = sum(loc_waiting[i] / prod_i[i] for i in range(len(data_mbf)))
print("Затраты (подход 2):", затраты_2, "чел.−мес")

# затраты_3 = sum(loc_waiting[i] / prod_i_an[i] for i in range(len(data)))
# print("Затраты (подход 3):", затраты_3, "чел.−мес")

удельная_стоимость_ср = sum(unit_cost) / len(unit_cost)

стоимость_1_2 = удельная_стоимость_ср * sum(loc_waiting)
print("Стоимость (подход 1,2):", стоимость_1_2, "у.е.")

стоимость_3 = sum(loc_waiting[i] * удельная_стоимость_ср for i in range(len(data_mbf)))
print("Стоимость (подход 3):", стоимость_3, "у.е.")
