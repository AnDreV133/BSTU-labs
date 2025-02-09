class decoder:
    encoded_data = ""

    def set_data_from_file(self, path_to_src):
        with open(path_to_src, 'r') as f:
            self.encoded_data = f.read()

    def set_data_from_string(self, src):
        self.encoded_data = src

    def get_decoded_data_for_send(self):
        return decoded_data

    def decoded_data_in_file(self, path):
        with open(path, 'w') as f:
            f.write(decoded_data)

    def decode_hamming_code(self):
        """Декодирует помехоустойчивый код Хэмминга.

        Args:
          encoded_data: Помехоустойчивый код Хэмминга.

        Returns:
          Декодированные исходные данные.
        """

        # Получаем размер кода
        n = len(self.encoded_data)

        # Вычисляем количество проверочных битов
        r = n - int(np.log2(n + 1))

        # Создаем матрицу проверочного паритета
        H = np.identity(r)
        for i in range(1, r):
            for j in range(i):
                H[i, j] = 1

        # Вычисляем синдром
        syndrome = np.dot(H, self.encoded_data) % 2

        # Проверяем на наличие ошибок
        if np.all(syndrome == 0):
            # Ошибок нет, возвращаем исходные данные
            return self.encoded_data[:n - r]

        # Находим позицию ошибки
        error_position = int(bin(syndrome[0])[2:], 2)

        # Исправляем ошибку
        self.encoded_data[error_position - 1] = 1 - self.encoded_data[error_position - 1]

        # Возвращаем декодированные исходные данные
        return self.encoded_data[:n - r]

    def lzw_decompress(compressed):
        dictionary = {i: chr(i) for i in range(256)}
        result = ""
        prev_buf = chr(compressed.pop(0))
        result += prev_buf
        for k in compressed:
            buf = ""
            if k in dictionary:
                buf = dictionary[k]
            elif k == len(dictionary):
                buf = prev_buf + prev_buf[0]
            result += buf
            dictionary[len(dictionary)] = prev_buf + buf[0]
            prev_buf = buf
        return result
