class encoder:
    data = ""

    def set_data_from_file(self, path_to_src):
        with open(path_to_src, 'r') as f:
            self.data = f.read()

    def set_data_from_string(self, src):
        self.data = src

    def get_encoded_data_for_send(self):
        return [metadata, encoded_data]

    def encoded_data_in_file(self, path):
        with open(path, 'w') as f:
            f.write(metadata + '\n')
            f.write(encoded_data)

    def lzw_compress_ascii(self):
        dictionary = {chr(i): i for i in range(256)}
        result = []
        buf = ""
        for c in self.data:
            wc = buf + c
            if wc in dictionary:
                buf = wc
            else:
                result.append(dictionary[buf])
                dictionary[wc] = len(dictionary)
                buf = c
        if buf:
            result.append(dictionary[buf])

        return result

    def gen_hamming_code(self, amount_control_bits_in_block):
        kb = amount_control_bits_in_block  # todo add split on block

        """Генерирует помехоустойчивый код Хэмминга.

            Args:
              data: Исходные данные для кодирования.

            Returns:
              Помехоустойчивый код Хэмминга.
            """

        # Получаем размер исходных данных
        n = len(self.data)

        # Вычисляем количество проверочных битов
        r = int(np.ceil(np.log2(n + 1)))

        # Создаем матрицу проверочного паритета
        H = np.identity(r)
        for i in range(1, r):
            for j in range(i):
                H[i, j] = 1

        # Добавляем проверочные биты к исходным данным
        encoded_data = np.zeros(n + r, dtype=int)
        encoded_data[:n] = self.data
        encoded_data[n:] = np.dot(H, encoded_data[:n]) % 2

        return encoded_data
