import pandas as pd
import matplotlib.pyplot as plt


def output_graph(path: str, name: str):
    # Загрузка данных из CSV-файла
    data = pd.read_csv(path, sep=";", skiprows=2,
                       names=["input_arg", "real_time", "user_time", "sys_time"])

    # Построение графиков
    plt.figure(figsize=(10, 6))

    # График real_time
    plt.plot(data["input_arg"], data["real_time"], label="real_time", marker="o")

    # График user_time
    plt.plot(data["input_arg"], data["user_time"], label="user_time", marker="s")

    # График sys_time
    plt.plot(data["input_arg"], data["sys_time"], label="sys_time", marker="^")

    # Настройка графика
    plt.title(f"{name}. Зависимость времени выполнения от количества итераций.")
    plt.xlabel("Количество итераций (input_arg)")
    plt.ylabel("Время (секунды)")
    plt.legend()  # Добавление легенды
    plt.grid(True)  # Включение сетки

    # Сохранение графика в файл
    plt.savefig(f"{name}.png")

    # Показать график
    plt.show()


# output_graph("./data1/timetest_static.csv", "static")
# output_graph("./data1/timetest_dynamic.csv", "dynamic")
# output_graph("./data1/timetest_guided.csv", "guided")
# output_graph("./data1/timetest_auto.csv", "auto")
#
# output_graph("./data2/timetest_static.csv", "static")
# output_graph("./data2/timetest_dynamic.csv", "dynamic")
# output_graph("./data2/timetest_guided.csv", "guided")
# output_graph("./data2/timetest_auto.csv", "auto")

# output_graph("./data1_fastedalg/timetest_static.csv", "static")
# output_graph("./data1_fastedalg/timetest_guided.csv", "guided")

output_graph("./data2_fastedalg/timetest_static.csv", "static")
output_graph("./data2_fastedalg/timetest_guided.csv", "guided")