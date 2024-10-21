class RecognizeException(Exception):
    def __init__(self, message):
        self.message = message

    def __str__(self):
        return str(self.message)


class Recognizer:
    YES = 'допустить'
    NO = 'отвергнуть'

    def check_inter(self, chain: str):
        S = 0
        for ch in chain:
            if S < 0:
                raise RecognizeException(self.ERROR_DICT[S])
            S = self.TABLE[self.L[ch]][S]
            print(S)
        return self.YES if S == 3 else self.NO

    def check_comp(self, chain: str):
        return self._S0(chain)

    def __init__(self):
        self.TABLE = [[2, -2, 3, 2],  # Апостроф
                      [-1, -2, 2, 0],  # Плюс
                      [-1, -2, 1, -3]]  # Диез
        self.TABLE += [[-1, 2, 2, -3] for i in range(0, 8)]  # Восьмиричные числа
        self.TABLE += [[-1, -2, 2, -3] for ch in self.ELSE]  # Все остальные символы

        self.L = {
            "'": 0,
            "+": 1,
            "#": 2
        }
        index = 3
        for ch in range(0, 8):
            self.L.update({str(ch): index})
            index += 1
        for ch in self.ELSE:
            self.L.update({str(ch): index})
            index += 1

    ERROR_DICT = {
        -1: 'Часть строки должна начинаться с апострофа',
        -2: 'После диеза должна идти цифра из восьмиричной системы счисления',
        -3: 'После апострофа должн идти апостроф или плюс'
    }

    APOSTROPH = "'"
    PLUS = '+'
    DIEZ = '#'
    EIGTH_DIGIT = '01234567'
    ELSE = 'йцукенгшщзхъэждлорпавыфячсмитьбю.,-!"№;%:?*()_=' \
           'qwertyuiop[];lkjhgf dsa'  # Множество всех входных символов за исключением тех что выше

    def _S0(self, chain: str):
        if not chain:
            return self.NO
        ch = chain[0]
        if ch in self.APOSTROPH:
            return self._S2(chain[1:])
        else:
            raise RecognizeException('Часть строки должна начинаться с апострофа')

    def _S1(self, chain: str):
        if not chain:
            return self.NO
        ch = chain[0]
        if ch in self.EIGTH_DIGIT:
            return self._S2(chain[1:])
        else:
            raise RecognizeException('После диеза должна идти цифра из восьмиричной системы счисления')

    def _S2(self, chain: str):
        if not chain:
            return self.NO
        ch = chain[0]
        if ch in self.APOSTROPH:
            return self._S3(chain[1:])
        elif ch in self.DIEZ:
            return self._S1(chain[1:])
        else:
            return self._S2(chain[1:])

    def _S3(self, chain: str):
        if not chain:
            return self.YES
        ch = chain[0]
        if ch in self.APOSTROPH:
            return self._S2(chain[1:])
        elif ch in self.PLUS:
            return self._S0(chain[1:])
        else:
            raise RecognizeException('После апострофа должн идти апостроф или плюс')


if __name__ == '__main__':
    recognizer = Recognizer()
    print('Вы хотите работать с консолью или файлом?')
    print('1 - консоль')
    print('2 - файл')

    flag = int(input())
    if flag == 1:
        print('Введите цепочку', end=' ')
        chain = input()

        print('Выберите тип реализации')
        print('1 - компиляционный')
        print('2 - интерпретационный')
        flag = int(input())
        if flag == 1:
            print(recognizer.check_comp(chain))
        elif flag == 2:
            print(recognizer.check_inter(chain))

    elif flag == 2:
        print('Введите путь к файлу')
        path = input()

        print('Введите ')
        print('1 - что бы удалить из файла все недопустимые цепочки')
        print('2 - что бы удалить из файла все допустимые цепочки')
        flag = int(input())

        with open(path) as f:
            all_chain = f.read().split('\n')
        delete_chain = []
        if flag == 1:
            for chain in all_chain:
                print(chain)
                try:
                    print(recognizer.check_comp(chain))
                except:
                    print('отвергнуть')
                    delete_chain.append(chain)
        if flag == 2:
            for chain in all_chain:
                print(chain)
                try:
                    print(recognizer.check_comp(chain))
                except:
                    print('отвергнуть')
                    continue
                delete_chain.append(chain)
        with open(path, 'w') as f:
            for chain in all_chain:
                if chain not in delete_chain:
                    f.write(chain + '\n')
