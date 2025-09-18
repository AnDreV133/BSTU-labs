import random
import math
from typing import List


def tweak_solution(solution: List[int], board_size: int) -> List[int]:
    """Модифицирует текущее решение"""
    new_solution = solution.copy()
    x_pos = random.randint(0, board_size - 1)
    y_pos = random.randint(0, board_size - 1)
    new_solution[x_pos] = y_pos
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


def log_info(file_name: str, bs_energy: List[int], bad_solution_change: List[int], temp_change: List[float]):
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


def start_learning(board_size: int, min_temp: float, max_temp: float, alpha: float, step_amount: int):
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


def main():
    # Параметры алгоритма
    BOARD_SIZE = 25
    MIN_TEMP = 0.5
    MAX_TEMP = 30.0
    ALPHA = 0.9999
    STEP_AMOUNT = 500

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


if __name__ == "__main__":
    main()