from itertools import product


def evaluate_expression(expression):
    try:
        return eval(expression)
    except ZeroDivisionError:
        return None


def find_expression(numbers, answer):
    numbers = list(numbers.replace(",", "").replace(" ", ""))

    for x in numbers:
        if not x.isdigit() or int(x) <= 0:
            raise Exception("Invalid input")

    operators = ["*", "/", "+", "-", ""]
    n = len(numbers)
    intermediate_result = None
    for ops in product(operators, repeat=n - 1):
        expr = ""
        for i in range(n - 1):
            if ops[i] == "":
                expr += f"{numbers[i]}{ops[i]}"
            else:
                expr = "(" + expr + f"{numbers[i]}){ops[i]}"

        expr += numbers[-1]
        if abs(evaluate_expression(expr) - answer) < 0.0001:
            expr = expr.replace("(", "").replace(")", "")
            intermediate_result = expr
            if ops.count("*") > 0 and ops.count("/") > 0:
                return expr

    return intermediate_result


def main():
    path = str(input("Введите путь к файлу: "))
    answer = float(input("Введите ответ: "))

    with open(path, "r") as file:
        numbers = file.read()

    expression = find_expression(numbers, answer)
    if expression:
        print("Найдено выражение:", expression)
    else:
        print("Выражение не найдено")


if __name__ == "__main__":
    main()
