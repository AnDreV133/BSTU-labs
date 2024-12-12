G_TERMINALS = ";[]a=<!()+*"

RULES = [
    ('S', 'OZ'),
    ('Z', ';OZ'),
    ('Z', ''),
    ('O', 'a1'),
    ('O', '!(Y)[S]0'),
    ('1', '=2'),
    ('1', '<a[S]0'),
    ('2', '(E8'),
    ('2', 'a3'),
    ('3', '[S]0'),
    ('3', ''),
    ('0', '[S]'),
    ('0', ''),
    ('Y', 'a7'),
    ('7', '=a'),
    ('7', '<a'),
    ('Y', '!(Y)'),
    ('E', '(E8'),
    ('8', '+E)'),
    ('8', '*E)'),
    ('E', 'a')
]


class MP:
    TABLE = {
        'S': {
            'a': RULES[0],
            '!': RULES[0]
        },
        'Z': {
            ';': RULES[1],
            ']': RULES[2],
            '┤': RULES[2],
        },
        'O': {
            'a': RULES[3],
            '!': RULES[4]
        },
        '1': {
            '=': RULES[5],
            '<': RULES[6],
        },
        '2': {
            '(': RULES[7],
            'a': RULES[8],
        },
        '3': {
            ';': RULES[10],
            '[': RULES[9],
            ']': RULES[10],
            '┤': RULES[10],
        },
        '0': {
            ';': RULES[12],
            '[': RULES[11],
            ']': RULES[12],
            '┤': RULES[12],
        },
        'Y': {
            'a': RULES[13],
            '!': RULES[16]
        },
        '7': {
            '=': RULES[14],
            '<': RULES[15]
        },
        'E': {
            '(': RULES[17],
            'a': RULES[20]
        },
        '8': {
            '+': RULES[18],
            '*': RULES[19]
        }
    }

    def _print_rule(self, rule: (str, str)):
        rule_num = RULES.index(rule) + 1
        print(f'{" " if rule_num < 10 else ""}{rule_num}. {rule[0]} -> {rule[1] if rule[1] else "e"}')

    def check(self, chain: str):
        stack = ['┤', 'S']
        chain += '┤'
        indexInSym = 0
        run_flag = True
        while run_flag:
            X = stack[-1]
            if (X in G_TERMINALS) or (X == '┤'):
                if X == chain[indexInSym]:
                    stack.pop()
                    indexInSym += 1
                else:
                    raise Exception
            else:
                try:
                    rule: (str, str) = MP.TABLE[X][chain[indexInSym]]  # тут вылетит ошибка если ячейка таблицы пуста
                    stack.pop()
                    stack += rule[1][::-1]
                    self._print_rule(rule)
                except:
                    raise Exception

            run_flag = (X != '┤')


tests = [
    "a=a[a=(a+a)];!(a<a)[a=(a*a)][a=a]",
    "a=a",
    # "a=a[a=(a+a)];",
    "a=a[",
    "!(a<a",
    # "O"
]
for test in tests:
    try:
        MP().check(test)
        print("correct")
    except Exception:
        print("incorrect")
    print()
