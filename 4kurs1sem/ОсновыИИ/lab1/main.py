import random
import math
from typing import List
import matplotlib.pyplot as plt
import numpy as np


def start_learning(
        board_size: int,
        min_temp: float,
        max_temp: float,
        alpha: float,
        step_amount: int
):
    """Запускает процесс обучения алгоритма отжига"""
    # Инициализация решений
    curr_solution = list(range(board_size))
    random.shuffle(curr_solution)
    best_solution = curr_solution.copy()

    # Подсчет коллизий
    cs_collision = count_collision(curr_solution, board_size)
    bs_collision = cs_collision

    # Логирование
    bs_energy = [bs_collision]
    bad_solution_change = []
    temp_change = []

    current_temp = max_temp
    found_solution = False

    while current_temp > min_temp and not found_solution:
        temp_change.append(current_temp)
        bad_solution_change_amount = 0

        for step in range(step_amount):
            # Модифицируем решение
            work_solution = tweak_solution(curr_solution, board_size)
            ws_collision = count_collision(work_solution, board_size)

            if ws_collision <= cs_collision:
                cs_collision = ws_collision
                curr_solution = work_solution.copy()

                if cs_collision < bs_collision:
                    best_solution = curr_solution.copy()
                    bs_collision = cs_collision
                    bs_energy.append(bs_collision)

                    if bs_collision == 0:
                        found_solution = True
                        break
            else:
                delta = ws_collision - cs_collision
                P = math.exp(-delta / current_temp)

                if random.random() < P:
                    cs_collision = ws_collision
                    curr_solution = work_solution.copy()
                    bad_solution_change_amount += 1

        bad_solution_change.append(bad_solution_change_amount)
        current_temp *= alpha

    # Вывод результатов
    print(f"Лучшее найденное решение: {[x + 1 for x in best_solution]}")
    print(f"Количество конфликтов: {bs_collision}")
    visualize_board(best_solution, board_size)

    return bs_energy, bad_solution_change, temp_change


def tweak_solution(
        solution: List[int],
        board_size: int
) -> List[int]:
    """Модифицирует текущее решение - меняет строку для случайного столбца"""
    new_solution = solution.copy()
    col = random.randint(0, board_size - 1)  # Выбираем случайный столбец

    # Генерируем новую позицию, отличную от текущей
    current_row = solution[col]
    possible_rows = [r for r in range(board_size) if r != current_row]

    if possible_rows:
        new_solution[col] = random.choice(possible_rows)

    return new_solution


def count_collision(solution: List[int], board_size: int) -> int:
    """Подсчитывает количество конфликтов (атак)"""
    collision = 0
    for i in range(board_size):
        for j in range(i + 1, board_size):
            # Проверка на диагональные атаки и атаки по вертикали
            if abs(i - j) == abs(solution[i] - solution[j]) or solution[i] == solution[j]:
                collision += 1
    return collision


def visualize_board(best_solution: List[int], board_size: int):
    """Визуализирует шахматную доску с ферзями"""
    print("\nШахматная доска:")
    for i in range(board_size):
        row = ""
        for j in range(board_size):
            if best_solution[j] == i:
                row += " Q "  # Ферзь
            else:
                row += " . "  # Пустая клетка
        print(row)
    print()


def log_info(
        file_name: str,
        bs_energy: List[int],
        bad_solution_change: List[int],
        temp_change: List[float]
):
    """Сохраняет данные в файл"""
    try:
        with open(file_name, 'w') as writer:
            # Энергия лучшего решения
            writer.write(','.join(map(str, bs_energy)) + ';\n')
            # Количество принятых плохих решений
            writer.write(','.join(map(str, bad_solution_change)) + ';\n')
            # Температура
            writer.write(','.join(map(str, temp_change)) + ';\n')

        print("Данные успешно записаны в файл")

    except IOError as e:
        print(f"Ошибка при записи в файл: {e}")


def plot_and_save_array(
        array,
        filename='graph.png',
        title='График',
        color='blue',
        xlabel='',
        ylabel=''
):
    """Строит и сохраняет график массива"""
    x = np.arange(len(array))

    plt.figure(figsize=(10, 6))
    plt.plot(x, array, color, linewidth=2, markersize=4)

    plt.title(title, fontsize=14)
    plt.xlabel(xlabel, fontsize=12)
    plt.ylabel(ylabel, fontsize=12)
    plt.grid(True, alpha=0.3)

    plt.savefig(filename, dpi=300, bbox_inches='tight')
    plt.close()

    print(f"График сохранен как '{filename}'")


def parse_log_file(filename):
    """Парсит лог-файл с данными алгоритма"""
    with open(filename, 'r') as file:
        lines = file.readlines()

    # Очищаем строки от лишних символов
    cleaned_lines = [line.strip().rstrip(';') for line in lines]
    cleaned_lines = [line[:-1] if line.endswith(',') else line for line in cleaned_lines]

    # Парсим данные
    best = [int(x) for x in cleaned_lines[0].split(',')]
    bad = [int(x) for x in cleaned_lines[1].split(',')]
    temp = [float(x) for x in cleaned_lines[2].split(',')]

    return best, bad, temp


def main(input_mode: bool = False, graph_mode: bool = False):
    if input_mode:
        # Ввод параметров
        BOARD_SIZE = int(input("Введите размер доски: "))
        MIN_TEMP = float(input("Введите минимальную температуру: "))
        MAX_TEMP = float(input("Введите максимальную температуру: "))
        ALPHA = float(input("Введите параметр альфа: "))
        STEP_AMOUNT = int(input("Введите количество шагов: "))
    else:
        # Параметры алгоритма
        BOARD_SIZE = 40
        MIN_TEMP = 0.5
        MAX_TEMP = 30.0
        ALPHA = 0.98
        STEP_AMOUNT = 1000

    # Запуск обучения
    bs_energy, bad_solution_change, temp_change = start_learning(
        board_size=BOARD_SIZE,
        min_temp=MIN_TEMP,
        max_temp=MAX_TEMP,
        alpha=ALPHA,
        step_amount=STEP_AMOUNT
    )

    # Сохранение логов
    log_info("log.txt", bs_energy, bad_solution_change, temp_change)

    if graph_mode:
        try:
            # Парсим данные из файла
            best, bad, temp = parse_log_file('log.txt')

            # Строим графики
            plot_and_save_array(array=best, filename="best.png",
                                title="График изменения лучшего решения",
                                color="blue",
                                xlabel="Номер лучшего решения",
                                ylabel="Энергия лучшего решения")

            plot_and_save_array(array=bad, filename="bad.png",
                                title="График количества принятых плохих решений на каждой итерации",
                                color="red",
                                xlabel="Номер итерации",
                                ylabel="Количество принятых плохих решений")

            plot_and_save_array(array=temp, filename="temp.png",
                                title="График изменения температуры",
                                color="orange",
                                xlabel="Номер итерации",
                                ylabel="Температура")

            print("Все графики успешно созданы!")

        except FileNotFoundError:
            print("Файл log.txt не найден!")
        except ValueError as e:
            print(f"Ошибка парсинга данных: {e}")
        except Exception as e:
            print(f"Произошла ошибка: {e}")


if __name__ == "__main__":
    main(
        input_mode=False,
        graph_mode=True
    )
