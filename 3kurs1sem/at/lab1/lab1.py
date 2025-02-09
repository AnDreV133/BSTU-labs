# from random import choices
#
#
# class G:
#     def __init__(self, n=set(), a=set(), p=dict(), s='S'):
#         self.N: set = n
#         self.A: set = a
#         self.P: dict = p
#         self.S: str = s
#         self.P_assotioate: list = []
#         for key in p:
#             for rule in p[key]:
#                 self.P_assotioate.append((rule[1], key))
#         self.P_assotioate.sort()
#         self.P_assotioate = [el[1] for el in self.P_assotioate]
#
#     def input_n(self):
#         print("Введите конечное множество нетерминальных символов", end=": ")
#         self.N.clear()
#         for n in input().split():
#             self.N.add(n)
#
#     def input_a(self):
#         print("Введите конечное множество терминальных символов", end=": ")
#         self.A.clear()
#         for a in input().split():
#             self.A.add(a)
#
#     def input_p(self):
#         self.P.clear()
#         self.P_assotioate.clear()
#         print("Введите колличество правил", end=": ")
#         count_p = int(input())
#         print("Введите %d правил:" % count_p)
#         for i in range(1, count_p + 1):
#             list_rule = input().split("->")
#             key = list_rule[0]
#             value = list_rule[1]
#             if key in self.P.keys():
#                 self.P[key].append([value, i])
#                 for _ in self.P[key]:
#                     self.P_assotioate.append(key)
#
#             else:
#                 self.P.update({key: [[value, i]]}.copy())
#
#     def input_s(self):
#         print("Введите начальный нетерминал", end=": ")
#         self.S = input()
#
#     def input_all(self):
#         self.input_n()
#         self.input_a()
#         self.input_p()
#         self.input_s()
#
#     def _get_non_term_rules(self, non_term):
#         rule_list = []
#         rights = self.P[non_term]
#         for r in rights:
#             rule_list.append(str(r[1]) + ". " + non_term + "->" + r[0])
#
#         rule_list.sort()
#
#         return rule_list
#
#     def __str__(self):
#         print("КС-грамматика: ")
#         rule_list = []
#         for non_term in self.P.keys():
#             rule_list += self._get_non_term_rules(non_term)
#
#         rule_list.sort()
#
#         return "\n".join(rule_list)
#
#     def left_tree(self, chain, list_rule):
#         offset = 0
#         for rule in list_rule:
#             newChain = chain[:offset]
#             for ch_i in range(offset, len(chain)):
#                 newChain += chain[ch_i]
#                 if chain[ch_i] in self.N:
#                     offset = len(newChain)
#                     newChain += "(" + rule[0] + ")"
#                     newChain += chain[ch_i + 1:]
#                     break
#
#             chain = newChain
#         return chain
#
#     def get_non_terminal_for_num_rule(self, num_rule) -> str:
#         return self.P_assotioate[num_rule - 1]
#
#     def check_rules(self, rules):
#         chain = self.S
#         for rule in rules:
#             non_term = self.get_non_terminal_for_num_rule(rule)
#             if non_term in chain:
#                 current_rule = list(filter(lambda el: el[1] == rule, self.P[non_term]))[0]
#                 chain = chain.replace(non_term, current_rule[0], 1)
#             else:
#                 return False
#
#         return True
#
#     def check_left_rules(self, rules):
#         chain = self.S
#         for rule in rules:
#             newChain = ""
#             flag = True
#             for ch in chain:
#                 if (ch in self.N) and flag:
#                     current_rule = list(filter(lambda el: el[1] == rule, self.P[ch]))
#                     flag = False
#                     if current_rule != []:
#                         newChain += current_rule[0][0]
#                     else:
#                         return False
#
#                 else:
#                     newChain += ch
#
#             chain = newChain
#
#         return True
#
#     def left_vivod(self):
#         print("Левый вывод\n")
#         chain = self.S
#         step = 1
#         step_by_step = []
#
#         while True:
#             chain.replace("e", "")
#             flag = True
#             newChain = ""
#             for ch in chain:
#                 if (ch in self.N) and flag:
#                     print("Шаг %d." % step)
#                     step += 1
#
#                     print("Промежуточная цепочка: %s" % chain)
#                     flag = False
#                     rules = self._get_non_term_rules(ch)
#                     print("Можно применить правила:")
#                     print("\n".join(rules))
#
#                     num = int(input())
#                     rule = list(filter(lambda el: el[1] == num, self.P[ch]))[0]
#                     print("Применяем правило %d" % rule[1])
#                     step_by_step.append(rule)
#                     newChain += rule[0]
#                 else:
#                     newChain += ch
#
#             print()
#
#             if flag: break
#             chain = newChain
#
#         chain.replace("e", "")
#         print("Шаг %d." % step)
#         print("Терминальная цепочка: %s" % chain)
#         print("Последовательность правил: %s" % ' '.join([str(step[1]) for step in step_by_step]))
#         print("ЛСФ ДВ: " + self.left_tree(self.S, step_by_step).replace("e", ""))
#
#
# if __name__ == "__main__":
#     G = G()
#     G.input_all()
#     print("Левый вывод")
#     G.left_vivod()

import re

txt = "abacaba"
x = re.findall("^(?=.*a)(?=.*b)(?=.*c)[abc]{4,k}$", txt)

print(x)