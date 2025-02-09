G_TERMINALS = ";[]a=<!()+*"


class Recurse:

    def __init__(self, chain: str):
        self.err_flag = False
        self.chain = chain + '┤'
        self.non_term = {
            'S': self.S,  # S
            'Z': self.St,  # S`
            'O': self.O,  # O
            '1': self.O1,  # O1
            '2': self.O2,  # O2
            '3': self.O3,  # O3
            '0': self.Ot,  # O`
            'Y': self.Y,  # Y
            '7': self.Yt,  # Y`
            'E': self.E,  # E
            '8': self.Et,  # E`
        }

    def S(self):
        CHANGE = {
            'a': 'OZ',
            '!': 'OZ',
        }
        try:
            in_sym = self.chain[0]
            print(f'S => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def St(self):
        CHANGE = {
            ';': ';OZ',
        }
        try:
            in_sym = self.chain[0]
            print(f'S` => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            if self.chain[0] in ']┤':
                print(f'S` => e')
            else:
                self.err_flag = True

    def O(self):
        CHANGE = {
            'a': 'a1',
            '!': '!(Y)[S]0'
        }
        try:
            in_sym = self.chain[0]
            print(f'O => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def O1(self):
        CHANGE = {
            '=': '=2',
            '<': '<a[S]0'
        }
        try:
            in_sym = self.chain[0]
            print(f'O1 => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def O2(self):
        CHANGE = {
            '(': '(E8',
            'a': 'a3'
        }
        try:
            in_sym = self.chain[0]
            print(f'O2 => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def O3(self):
        CHANGE = {
            '[': '[S]0'
        }
        try:
            in_sym = self.chain[0]
            print(f'O3 => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            if self.chain[0] in ';]┤':
                print(f'O3 => e')
            else:
                self.err_flag = True

    def Ot(self):
        CHANGE = {
            '[': '[S]'
        }
        try:
            in_sym = self.chain[0]
            print(f'O` => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            if self.chain[0] in ';]┤':
                print(f'O` => e')
            else:
                self.err_flag = True

    def Y(self):
        CHANGE = {
            'a': 'a7',
            '!': '!(Y)'
        }
        try:
            in_sym = self.chain[0]
            print(f'Y => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def Yt(self):
        CHANGE = {
            '=': '=a',
            '<': '<a'
        }
        try:
            in_sym = self.chain[0]
            print(f'Y` => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def E(self):
        CHANGE = {
            '(': '(E8',
            'a': 'a'
        }
        try:
            in_sym = self.chain[0]
            print(f'E => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def Et(self):
        CHANGE = {
            '+': '+E)',
            '*': '*E)'
        }
        try:
            in_sym = self.chain[0]
            print(f'E` => {CHANGE[in_sym]}')
            self.parse(CHANGE[in_sym])
        except Exception:
            self.err_flag = True

    def parse(self, u: str):
        if self.err_flag:
            return

        v = u
        while v:
            X = v[0]
            z = v[1::]
            if X in G_TERMINALS:
                if X != self.chain[0]:
                    raise Exception
                else:
                    self.chain = self.chain[1::]
            else:
                self.non_term[X]()
            v = z


tests = [
    "a=a[a=(a+a)];!(a<a)[a=(a*a)][a=a]",
    "a=a",
    # "a=a[a=(a+a)];",
    "a=a[",
    "!(a<a",
    # "O"
]
for test in tests:
    r = Recurse(test)
    try:
        r.S()
    except Exception:
        pass
    if not r.err_flag:
        print("correct")
    else:
        print("incorrect")
    print()

# S => OZ; O => a1; O1 => =2; O2 => a3; O3 => [S]0; S => OZ; O => a1; O1 => =2; O2 => (E8; E => a; E => +E); E => a; S` => e; O` => e; S` => ;OZ; O => !(Y)[S]0; Y => a7; Y` => <a; S => OZ; O => a1; O1 => =2; O2 => (E8; E => a; E => *E); E => a; S` => e; O` => [S]; S => OZ; O => a1; O1 => =2; O2 => a3; O3 => e; S` => e; S` => e
