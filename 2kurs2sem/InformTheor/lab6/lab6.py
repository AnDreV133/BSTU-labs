import random


def hartley_source(length):
    return ''.join(str(random.randint(0, 1)) for _ in range(length))


def get_data(path):
    with open(path, 'r') as f:
        return f.read()


# кодирование
def lzw_compress(message):
    dictionary = {chr(i): i for i in range(256)}
    result = []
    buf = ""
    for c in message:
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


# декодирование
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


hartley_data_1k = hartley_source(1000)
hartley_data_10k = hartley_source(10000)
text_data = get_data('msg10k.txt')

# # Применение LZW к сообщению и декомпрессия
# print("Исходное сообщение длиной 1000 символов (Хартли):")
# print(hartley_data_1k)
# compressed_message_1000 = lzw_compress(hartley_data_1k)
# print("Сообщение длиной 1000 символов (Хартли) с использованием LZW:")
# print(compressed_message_1000)
# decompressed_message_1000 = lzw_decompress(compressed_message_1000)
# print("Декодированное сообщение длиной 1000 символов (Хартли):")
# print(decompressed_message_1000)

print("Исходное сообщение длиной 10000 символов (Бернулли):")
print(hartley_data_10k)
compressed_message_10000 = lzw_compress(hartley_data_10k)
print("Сообщение длиной 10000 символов (Бернулли) с использованием LZW:")
print(compressed_message_10000)
decompressed_message_10000 = lzw_decompress(compressed_message_10000)
print("Декодированное сообщение длиной 10000 символов (Бернулли):")
print(decompressed_message_10000)

# print("Исходное сообщение длиной 10000 символов (текст):")
# print(text_data)
# compressed_message_10000_different = lzw_compress(text_data)
# print("Сообщение длиной 10000 символов (текст) с использованием LZW:")
# print(compressed_message_10000_different)
# decompressed_message_10000_different = lzw_decompress(compressed_message_10000_different)
# print("Декодированное сообщение длиной 10000 символов (текст):")
# print(decompressed_message_10000_different)

# print("\n-------------------------------Замер-эффективноти-сжатия-------------------------------\n")

# def compression_ratio(original_size, compressed_size):
#     return original_size / compressed_size
#
#
# # Оценка коэффициента сжатия для сообщений различных размеров и типов
# original_size_1000 = len(hartley_data_1k)
# original_size_10000 = len(hartley_data_10k)
# original_size_10000_different = len(text_data)
#
# compressed_size_1000 = len(compressed_message_1000)
# compressed_size_10000 = len(compressed_message_10000)
# compressed_size_10000_different = len(compressed_message_10000_different)
#
# compression_ratio_1000 = compression_ratio(original_size_1000, compressed_size_1000)
# compression_ratio_10000 = compression_ratio(original_size_10000, compressed_size_10000)
# compression_ratio_10000_different = compression_ratio(original_size_10000_different, compressed_size_10000_different)
#
# print("Коэффициент сжатия для сообщения длиной 1000 символов (Хартли):", compression_ratio_1000)
# print("Коэффициент сжатия для сообщения длиной 10000 символов (Хартли):", compression_ratio_10000)
# print("Коэффициент сжатия для сообщения длиной 10000 символов (текст):", compression_ratio_10000_different)
#

