import pandas as pd


def средняя_производительность():
    res = []
    for i in range(len(data)):
        loc = float(data['loc'][i])
        spending = int(data['spending'][i])
        res.append(loc / spending)

    return sum(res) / len(res)


def средняя_loc():
    res = list(map(float, data['loc']))

    return sum(res) / len(res)


delimiter_length = 70

data = pd.read_csv(
    "data.csv",
    sep=';'
)
произв_ср = средняя_производительность()
loc_ср = средняя_loc()

buf: dict = {
    "func": [],
    "loc_waiting": [],
    "prod_avg": [],
    "prod_i": [],
    "prod_i_an": [],
    "unit_cost": []
}
for i in range(len(data)):
    proj = str(data['proj'][i])
    loc = float(data['loc'][i])
    spending = int(data['spending'][i])
    cost = float(data['cost'][i])
    errors = int(data['errors'][i])
    doc = int(data['doc'][i])
    func = int(data['func'][i])
    loc_best = float(data['loc_best'][i])
    loc_top = float(data['loc_top'][i])
    loc_worst = float(data['loc_worst'][i])

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

    print("-" * delimiter_length)

    print("Функция", func)

    loc_ожидания = (loc_best + loc_worst + 4 * loc_top) / 6
    print("LOC ожидания:", loc_ожидания, "тыс.LOC")

    print("Производительность (подход 1):", произв_ср, "тыс.LOC/чел.-мес")

    произв_i = произв_ср * loc_ср / loc_ожидания
    print("Производительность (подход 2):", произв_i, "тыс.LOC/чел.-мес")

    # произв_i_ан = производительность * loc_ср / loc_ожидания
    # print("Производительность (подход 3):", произв_i_ан, "тыс.LOC/чел.-мес")

    buf['func'].append(func)
    buf['loc_waiting'].append(loc_ожидания)
    buf['prod_avg'].append(произв_ср)
    buf['prod_i'].append(произв_i)
    # buf['prod_i_an'].append(произв_i_ан)
    buf['unit_cost'].append(удельная_стоимость)

func = buf['func']
loc_waiting = buf['loc_waiting']
prod_avg = buf['prod_avg']
prod_i = buf['prod_i']
# prod_i_an = buf['prod_i_an']
unit_cost = buf['unit_cost']

print("~" * delimiter_length)

затраты_1 = sum(loc_waiting) / произв_ср
print("Затраты (подход 1):", затраты_1, "чел.−мес")

затраты_2 = sum(loc_waiting[i] / prod_i[i] for i in range(len(data)))
print("Затраты (подход 2):", затраты_2, "чел.−мес")

# затраты_3 = sum(loc_waiting[i] / prod_i_an[i] for i in range(len(data)))
# print("Затраты (подход 3):", затраты_3, "чел.−мес")

удельная_стоимость_ср = sum(unit_cost) / len(unit_cost)

стоимость_1_2 = удельная_стоимость_ср * sum(loc_waiting)
print("Стоимость (подход 1,2):", стоимость_1_2, "у.е.")

стоимость_3 = sum(loc_waiting[i] * удельная_стоимость_ср for i in range(len(data)))
print("Стоимость (подход 3):", стоимость_3, "у.е.")
