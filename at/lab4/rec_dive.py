# G_NOT_TERMINALS = 'SZOPYWE'
G_TERMINALS = '{[a=<!+*]}(,)'


class ParseException(Exception): pass


class Recurse:
    def __init__(self, chain: str):
        self.chain = chain

    def S(self):
        FIRST = {
            '{': 'OZ',
            'a': 'OZ',
        }
        try:
            in_sym = self.chain[0]
            print(f'S -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def Z(self):
        FIRST = {
            '{': 'S',
            'a': 'S'
        }
        try:
            in_sym = self.chain[0]
            print(f'Z -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            if self.chain[0] in ']$':
                print(f'Z -> e')
            else:
                raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def O(self):
        FIRST = {
            '{': '{P',
            'a': 'a=E'
        }
        try:
            in_sym = self.chain[0]
            print(f'O -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def P(self):
        FIRST = {
            '[': '[S]Y}',
            'a': 'Y[S]}',
            '!': 'Y[S]}'
        }
        try:
            in_sym = self.chain[0]
            print(f'P -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def Y(self):
        FIRST = {
            'a': 'aW',
            '!': '!(Y)'
        }
        try:
            in_sym = self.chain[0]
            print(f'Y -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def W(self):
        FIRST = {
            '=': '=a',
            '<': '<a'
        }
        try:
            in_sym = self.chain[0]
            print(f'Y -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def E(self):
        FIRST = {
            'a': 'a',
            '+': '+(E,E)',
            '*': '*(E,E)'
        }
        try:
            in_sym = self.chain[0]
            print(f'E -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def parse(self, u: str):
        v = u
        while v:
            X = v[0]
            z = v[1::]
            if X in G_TERMINALS:
                if X != self.chain[0]:
                    raise ParseException()
                else:
                    self.chain = self.chain[1::]
            else:
                {
                    'S': self.S,
                    'Z': self.Z,
                    'O': self.O,
                    'P': self.P,
                    'Y': self.Y,
                    'W': self.W,
                    'E': self.E,
                }[X]()
            v = z


G_NOT_TERMINALS = 'SZOPYWE'
G_TERMINALS = '{[a=<!+*]}(,)'


class Recurse:

    def __init__(self, chain: str):
        self.chain = chain

    def S(self):
        FIRST = {
            '{': 'OZ',
            'a': 'OZ',
        }
        try:
            in_sym = self.chain[0]
            print(f'S -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def Z(self):
        FIRST = {
            '{': 'S',
            'a': 'S'
        }
        try:
            in_sym = self.chain[0]
            print(f'Z -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            if self.chain[0] in ']$':
                print(f'Z -> e')
            else:
                raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def O(self):
        FIRST = {
            '{': '{P',
            'a': 'a=E'
        }
        try:
            in_sym = self.chain[0]
            print(f'O -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def P(self):
        FIRST = {
            '[': '[S]Y}',
            'a': 'Y[S]}',
            '!': 'Y[S]}'
        }
        try:
            in_sym = self.chain[0]
            print(f'P -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def Y(self):
        FIRST = {
            'a': 'aW',
            '!': '!(Y)'
        }
        try:
            in_sym = self.chain[0]
            print(f'Y -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def W(self):
        FIRST = {
            '=': '=a',
            '<': '<a'
        }
        try:
            in_sym = self.chain[0]
            print(f'Y -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def E(self):
        FIRST = {
            'a': 'a',
            '+': '+(E,E)',
            '*': '*(E,E)'
        }
        try:
            in_sym = self.chain[0]
            print(f'E -> {FIRST[in_sym]}')
            self.parse(FIRST[in_sym])
        except:
            raise ParseException()  # В данном случае из нетерминала не выводится пустая цепочка

    def parse(self, u: str):
        v = u
        while v:
            X = v[0]
            z = v[1::]
            if X in G_TERMINALS:
                if X != self.chain[0]:
                    raise ParseException()
                else:
                    self.chain = self.chain[1::]
            else:
                {
                    'S': self.S,
                    'Z': self.Z,
                    'O': self.O,
                    'P': self.P,
                    'Y': self.Y,
                    'W': self.W,
                    'E': self.E,
                }[X]()
            v = z
