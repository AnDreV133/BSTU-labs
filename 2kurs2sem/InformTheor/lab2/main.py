N = 5


class Node:
    def __init__(self, freqs):
        self.freqs = freqs
        self.left = None
        self.right = None


# рассчет вероятностей символов
def calculate_freq(s):
    # подсчет кол-ва каждого символа в строке
    freqs = {}
    for w in s:
        freqs[w] = 0

    for w in s:
        freqs[w] += 1

    # расчет вероятностей
    for symb in freqs:
        freqs[symb] = round(freqs[symb] / len(s), 3)

    freqs = sorted(freqs.items(), key=lambda item: item[1], reverse=True)

    return freqs


# подсчет сумм вероятностей переданного списка
def calculate_freqs_sum(d):
    c = 0

    dd = dict(d)
    for v in dd.values():
        c += v

    return c


def printCodes(shannon_codes):
    print("\nПолученные коды:")

    for key, value in shannon_codes.items():
        print(f"{key} : {value}")


def printFreqs(freqs):
    print("Таблица частот(вероятностей):")
    for el in freqs:
        print(f"{el[0]} : {el[1]}")


def encode(input_s, shannon_codes):
    encoded_s = ""

    for symb in input_s:
        encoded_s += shannon_codes[symb]

    return encoded_s


# расчет коэффициента сжатия
def calculateKcomp(input_s, encoded_s):
    return len(input_s) * N / len(encoded_s)


# расчет дисперсии
def calculateDispersion(freqs, shannonCodes):
    dispersion = 0

    # рассчитаем Lср
    l_middle = 0
    for f in freqs:

        l_middle += f[1] * len(shannonCodes[f[0]])

    for f in freqs:
        dispersion += f[1] * ((len(shannonCodes[f[0]]) - l_middle) ** 2)

    return dispersion


# проходом по дереву Шеннона-Фано получаем коды
def get_codes(node, s, shannon_codes):
    if node.right is None:
        shannon_codes[node.freqs[0][0]] = s
        return

    get_codes(node.left, s + "0", shannon_codes)
    get_codes(node.right, s + "1", shannon_codes)


# генерация дерева Шеннона-Фано
def createShannonTree(node):
    if len(node.freqs) == 1:
        return

    freqs_sum = 0
    mid_freqs_sum = calculate_freqs_sum(node.freqs) / 2

    # поиск разделительного элемента в списке
    sep_index = -1
    for i in range(len(node.freqs)):
        freqs_sum += node.freqs[i][1]

        if freqs_sum >= mid_freqs_sum:
            if (freqs_sum - mid_freqs_sum) <= (mid_freqs_sum - (freqs_sum - node.freqs[i][1])):
                # freqs[i] - разделитель
                sep_index = i

            else:
                # freqs[i-1] - разделитель
                sep_index = i - 1

            break

    node.right = Node(node.freqs[:sep_index + 1])
    node.left = Node(node.freqs[sep_index + 1:])

    createShannonTree(node.right)
    createShannonTree(node.left)


def main():
    input_s = input("Введите текст: ")

    print(f"\nВведенный текст: (длина = {len(input_s)})\n{input_s}\n")

    freqs = calculate_freq(input_s)

    root = Node(freqs)

    printFreqs(freqs)

    createShannonTree(root)

    shannon_codes = {}

    get_codes(root, "", shannon_codes)

    printCodes(shannon_codes)

    encoded_s = encode(input_s, shannon_codes)

    print(f"\nЗакодированное сообщение: (длина = {len(encoded_s)})\n{encoded_s}")

    print(f"\nКоэффициент сжатия: {calculateKcomp(input_s, encoded_s)}")
    print(f"\nДисперсия: {calculateDispersion(freqs, shannon_codes)}")


main()
