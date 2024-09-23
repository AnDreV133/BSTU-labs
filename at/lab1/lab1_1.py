class GMode:
    LEFT_OUTPUT = 1
    RIGHT_OUTPUT = 2
    FREE_OUTPUT = 3


class G:
    def __init__(self, n, a, p, s):
        self.N: list = n
        self.A: list = a
        self.P: dict[str, list[tuple[str, int]]] = p
        self.S: str = s
        self.seq: str = s
        self.rules_history: str = ""
        self.tree: str = s
        self.output: str = s
        self.output_rules = ""

    def clear(self):
        self.seq = self.S
        self.rules_history = ""
        self.tree = self.S
        self.output = self.S
        self.output_rules = ""

    def to_str_rule(self, non_term, rule):
        return str(rule[1]) + ". " + non_term + "->" + rule[0]

    def show_valid_rule(self, get_non_term):
        res: set[str] = set()
        for undef_term in self.seq:
            non_term = get_non_term()
            if undef_term.isupper() and undef_term in non_term:
                rights = self.P[undef_term]
                for right in rights:
                    res.add(self.to_str_rule(undef_term, right))

        _res = list(res)
        _res.sort()
        for x in _res:
            print(x)

    def get_tree(self):
        res = self.tree
        for i in range(len(self.N)):
            res = res.replace(str(i), self.N[i])

        return res

    def check_on_term(self):
        for x in self.seq:
            if x.isupper():
                return False

        return True

    def define_rule(self, rule_num):
        # rule: tuple[str, str] = tuple[str, str]()
        rule = None
        for rules in self.P.items():
            for pair in rules[1]:
                if pair[1] == rule_num:
                    rule = (rules[0], pair[0])
                    break

            if rule: break

        if not rule: raise Exception("rule not found by num")

        return rule

    def get_left_non_term(self):
        for x in self.seq:
            if x.isupper():
                return x

        return None

    def get_right_non_term(self):
        temp = list(self.seq)
        temp.reverse()
        for x in temp:
            if x.isupper():
                return x

        return None

    def get_free_non_term(self):
        res = set()
        for x in self.seq:
            if x.isupper():
                res.add(x)

        return res

    def use_rule(self, rule_num, get_non_term):
        rule = self.define_rule(rule_num)

        non_term = get_non_term()
        if not rule[0] in non_term or not non_term:
            raise Exception("non-term not found")

        temp = (
            self.seq
            .replace(rule[0], rule[1], 1)
            .replace("e", "")
        )
        if temp != self.seq:
            self.rules_history += str(rule_num) + ' '
            self.output += " => " + temp
            self.output_rules += str(rule_num) + (' ' * (4 + len(self.seq) - 1))
            self.tree = (
                self.tree
                .replace(rule[0], "%s(%s)" % (self.N.index(rule[0]), rule[1]), 1)
            )

            self.seq = temp
        else:
            raise Exception("rule not use")

    def run_process(self, mode: int, seq_of_command: list[int] = ()):
        i = 0
        command = -1
        while command != 0:
            prev_seq = self.seq

            if self.check_on_term():
                print("Терминальная цепочка:", self.seq)
                print("Последовательность правил:", self.rules_history)
                print("ЛСФ ДВ:", self.get_tree())
                print("Вывод:")
                print(self.output)
                print(self.output_rules)
                break

            print()
            print("Шаг %s" % str(i + 1))
            print("Промежуточная цепочка: %s" % self.seq)
            print("Можно применить правила:")
            if mode == GMode.LEFT_OUTPUT:
                self.show_valid_rule(self.get_left_non_term)
            elif mode == GMode.RIGHT_OUTPUT:
                self.show_valid_rule(self.get_right_non_term)
            elif mode == GMode.RIGHT_OUTPUT:
                self.show_valid_rule(self.get_free_non_term)
            else:
                raise Exception("GMode not change")

            if i >= len(seq_of_command):
                command = int(input())
            else:
                command = seq_of_command[i]

            if mode == GMode.LEFT_OUTPUT:
                self.use_rule(command, self.get_left_non_term)
            elif mode == GMode.RIGHT_OUTPUT:
                self.use_rule(command, self.get_right_non_term)
            elif mode == GMode.RIGHT_OUTPUT:
                self.use_rule(command, self.get_free_non_term)
            else:
                raise Exception("GMode not change")

            # self.rules_history += str(command) + ' '
            # self.output += " => " + self.seq
            # self.output_rules += str(command) + (' ' * (4 + len(prev_seq) - 1))

            i += 1

    def check_on_term_by_rules(self, rule_nums, get_non_term):
        curr_rule_num = 0
        try:
            for rule_num in rule_nums:
                curr_rule_num = rule_num
                self.use_rule(rule_num, get_non_term)
        except:
            self.rules_history += '~' + str(curr_rule_num)
            return False

        return self.check_on_term()

    def run_check_process(self, mode: int, seq_of_command: list[int] = ()):
        print("КС-грамматика:")
        for non_term in self.P.items():
            for pair in non_term[1]:
                print(self.to_str_rule(non_term[0], pair))

        if len(seq_of_command) == 0:
            seq_of_command = list(map(int, input().split()))

        if mode == GMode.LEFT_OUTPUT:
            res = self.check_on_term_by_rules(seq_of_command, self.get_left_non_term)
        elif mode == GMode.RIGHT_OUTPUT:
            res = self.check_on_term_by_rules(seq_of_command, self.get_right_non_term)
        elif mode == GMode.FREE_OUTPUT:
            res = self.check_on_term_by_rules(seq_of_command, self.get_free_non_term)
        else:
            raise Exception("GMode not change")

        print("Введенная последовательность правил:", *seq_of_command)
        print("Примененная последовательность правил:", self.rules_history)
        print("Результат:", "Да" if res else "Нет")
        print("Вывод:")
        print(self.output)
        print(self.output_rules)


if __name__ == "__main__":
    g = G(
        n=['S', 'A', 'B'],
        a=['a', 'b'],
        p={
            'S': [("Aba", 1), ("bS", 2), ("cAbA", 3)],
            'A': [("AaS", 4), ("BaB", 5), ("e", 6)],
            'B': [("b", 7), ("aA", 8)]
        },
        s='S'
    )

    # g.run_process(GMode.LEFT_OUTPUT, [
    #     1, 5,  8, 5, 8, 6, 8, 6,  8, 5, 8, 6, 8, 6,
    # ])  # aaaaaaaaaba
    # g.clear()
    # g.run_process(GMode.LEFT_OUTPUT, [
    #     1, 5,  8, 5,  8, 5,  8, 6, 8, 6, 8, 6, 8, 6,
    # ])  # ааааааbа

    g.run_process(GMode.LEFT_OUTPUT, [
        1, 5, 8, 5, 8, 6, 8, 6, 8, 6
    ])  # aaaaaaaaaba
    g.clear()
    g.run_process(GMode.LEFT_OUTPUT, [
        1, 5, 8, 6, 8, 5, 8, 6, 8, 6
    ])  # ааааааbа

    # g.run_check_process(GMode.RIGHT_OUTPUT,[1, 4, 7, 5, 3, 7, 7, 6, 6, 4, 6, 2, 3, 6, 6])
    # g.clear()
    # print()
    # g.run_check_process(GMode.RIGHT_OUTPUT,[1, 7, 4, 3, 6, 6, 5, 7, 4, 2, 3, 6, 6, 6, 7])
    # g.clear()
    # print()
    # g.run_check_process(GMode.RIGHT_OUTPUT,[1, 4, 5, 7, 7, 4, 6, 2, 1, 6, 7, 3, 6, 6, 7])
    # g.clear()
    # print()
    # g.run_check_process(GMode.RIGHT_OUTPUT,[1, 4, 3, 5, 7, 6, 6, 7, 7, 4, 2, 6, 6, 7, 6])
    # g.run_check_process(GMode.LEFT_OUTPUT, [1, 5, 8, 5, 8, 6, 8, 6, 8, 6])
    # g.clear()
    # print()
    # print()

    # seq = [1, 5, 8, 6, 8, 5, 8, 6, 8, 6]
    # # seq = [1, 5, 8, 8, 5, 6, 8, 8, 6, 6]
    # g.run_check_process(GMode.FREE_OUTPUT, seq)
    # g.clear()
    # g.run_check_process(GMode.LEFT_OUTPUT, seq)
    # g.clear()
    # g.run_check_process(GMode.RIGHT_OUTPUT, seq)
