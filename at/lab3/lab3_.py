task = 1

MESSAGES = {
    -1: "Отвергнуть. Последовательность пуста",
    -2: "Отвергнуть. Входной символ невалидный",
    -3: "Отвергнуть. Символ b должен быть введён не более 1 раза",
    -4: "Отвергнуть. Символ а должен быть введён не более 1 раза",
    -5: "Отвергнуть. Символ b должен быть введён хотя бы 1 раз",
    -6: "Отвергнуть. Символ a должен быть введён хотя бы 1 раз",
    0: "Допустить."
}


def S1(input):
    if len(input) == 0:
        return -1

    if input[0] == 'a':
        return S2(input[1:])
    elif input[0] == 'b':
        return S6(input[1:])
    else:
        return -2


def S2(input):
    if len(input) == 0:
        return -5

    if input[0] == 'a':
        return S3(input[1:])
    elif input[0] == 'b':
        return S4(input[1:])
    else:
        return -2


def S3(input):
    _input = input
    while True:
        if len(_input) == 0:
            return -5

        if _input[0] == 'a':
            _input = _input[1:]
        elif _input[0] == 'b':
            return S5(_input[1:])
        else:
            return -2


def S5(input):
    _input = input
    while True:
        if len(_input) == 0:
            return 0

        if _input[0] == 'a':
            _input = _input[1:]
        elif _input[0] == 'b':
            return -3
        else:
            return -2


def S4(input):
    if len(input) == 0:
        return 0

    if input[0] == 'a':
        return S5(input[1:])
    elif input[0] == 'b':
        return S9(input[1:])
    else:
        return -2


def S6(input):
    if len(input) == 0:
        return -6

    if input[0] == 'a':
        return S4(input[1:])
    elif input[0] == 'b':
        return S7(input[1:])
    else:
        return -2


def S7(input):
    _input = input
    while True:
        if len(_input) == 0:
            return -6

        if _input[0] == 'a':
            return S9(_input[1:])
        elif _input[0] == 'b':
            _input = _input[1:]
        else:
            return -2


def S9(input):
    _input = input
    while True:
        if len(_input) == 0:
            return 0

        if _input[0] == 'a':
            return -4
        elif _input[0] == 'b':
            _input = _input[1:]
        else:
            return -2


def L3validator(input):
    result = S1(input)
    return result


if task == 0:
    check_set = [
        ("aaaaabaaaa", 1),
        ("bbbbbabbbb", 1),
        ("ab", 1),
        ("ba", 1),
        ("aab", 1),
        ("bba", 1),
        ("aba", 1),
        ("bab", 1),
        ("", 0),
        ("aabb", 0),
        ("bbaa", 0),
        ("aaabaab", 0),
        ("bbbabba", 0),
        ("cccc", 0),
    ]

    check_set = [
        ("aaab", 1),
        ("aab", 1),
        ("aaba", 1),
        ("ab", 1),
        ("aba", 1),
        ("abaa", 1),
        ("ba", 1),
        ("bab", 1),
        ("babb", 1),
        ("bbba", 1),
        ("bba", 1),
        ("bbbab", 1),
        ("bbab", 1),
        ("", 0),  # 1
        ("c", 0),  # 1
        ("a", 0),  # 2
        ("aaa", 0),  # 3
        ("ab", 1),  # 4
        ("ba", 1),  # 4
        ("aab", 1),  # 5
        ("aba", 1),  # 5
        ("baa", 1),  # 5
        ("aabb", 0),  # 5
        ("b", 0),  # 6
        ("bb", 0),  # 7
        ("bba", 1),  # 9
        ("bab", 1),  # 9
        ("abb", 1),  # 9
        ("bbaa", 0)  # 9
    ]

    for check in check_set:
        res = L3validator(check[0])
        if check[1]:
            print(check[0], res >= 0, MESSAGES[res], sep='\t')
        else:
            print(check[0], res < 0, MESSAGES[res], sep='\t')

CONVERT = {
    2: -5,
    3: -5,
    6: -6,
    7: -6,
    -1: -1,
    -2: -2,
    -3: -3,
    -4: -4,
    -5: -5,
    -6: -6,
}

MESSAGES = {
    -1: "Отвергнуть. Последовательность пуста",
    -2: "Отвергнуть. Входной символ невалидный",
    -3: "Отвергнуть. Символ b должен быть введён не более 1 раза",
    -4: "Отвергнуть. Символ а должен быть введён не более 1 раза",
    -5: "Отвергнуть. Символ b должен быть введён хотя бы 1 раз",
    -6: "Отвергнуть. Символ a должен быть введён хотя бы 1 раз",
    0: "Допустить",
}

PERMITTING = [4, 5, 8]

MATRIX = {
    'a': [2, 3, 3, 5, 5, 4, 8, -4],
    'b': [6, 4, 5, 8, -3, 7, 7, 8],
}


def L3validator(input):
    if len(input) == 0:
        return -1

    S = 1
    while len(input) > 0 and S > 0:
        if not input[0] in MATRIX.keys():
            return -2
        S = MATRIX[input[0]][S - 1]
        input = input[1:]

    if S in PERMITTING:
        return 0

    return S


if task == 1:
    check_set = [
        ("aaab", 1),
        ("aab", 1),
        ("aaba", 1),
        ("ab", 1),
        ("aba", 1),
        ("abaa", 1),
        ("ba", 1),
        ("bab", 1),
        ("babb", 1),
        ("bbba", 1),
        ("bba", 1),
        ("bbbab", 1),
        ("bbab", 1),
        ("", 0),  # 1
        ("c", 0),  # 1
        ("a", 0),  # 2
        ("aaa", 0),  # 3
        ("ab", 1),  # 4
        ("ba", 1),  # 4
        ("aab", 1),  # 5
        ("aba", 1),  # 5
        ("baa", 1),  # 5
        ("aabb", 0),  # 5
        ("b", 0),  # 6
        ("bb", 0),  # 7
        ("bba", 1),  # 9
        ("bab", 1),  # 9
        ("abb", 1),  # 9
        ("bbaa", 0)  # 9
    ]
    for check in check_set:
        res = L3validator(check[0])
        if check[1]:
            print(check[0], res == 0, MESSAGES[res], sep='\t')
        else:
            print(check[0], res != 0, MESSAGES[CONVERT[res]], sep='\t')
