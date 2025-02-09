import heapq
from collections import defaultdict


class Node:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq


def build_huffman_tree(text):
    # Подсчет частоты символов в тексте
    freq_map = defaultdict(int)
    for char in text:
        freq_map[char] += 1

    # Создание очереди с приоритетом из узлов-листьев
    priority_queue = []
    for char, freq in freq_map.items():
        heapq.heappush(priority_queue, Node(char, freq))

    # Строим дерево Хаффмана
    while len(priority_queue) > 1:
        left_node = heapq.heappop(priority_queue)
        right_node = heapq.heappop(priority_queue)
        merged_node = Node(None, left_node.freq + right_node.freq)
        merged_node.left = left_node
        merged_node.right = right_node
        heapq.heappush(priority_queue, merged_node)

    return priority_queue[0], freq_map


def build_huffman_codes(node, current_code, huffman_codes):
    if node is None:
        return

    # Если узел - лист, то добавляем его код в словарь кодов Хаффмана
    if node.char is not None:
        huffman_codes[node.char] = current_code
        return

    # Рекурсивно строим коды для левого и правого поддеревьев
    build_huffman_codes(node.left, current_code + "0", huffman_codes)
    build_huffman_codes(node.right, current_code + "1", huffman_codes)


def huffman_encoding(text):
    root, freq_map = build_huffman_tree(text)
    huffman_codes = {}
    build_huffman_codes(root, "", huffman_codes)

    encoded_text = ""
    for char in text:
        encoded_text += huffman_codes[char]

    return encoded_text, huffman_codes, freq_map


def huffman_decoding(encoded_text, huffman_codes):
    decoded_text = ""
    shift = 0
    while shift < len(encoded_text):
        for val, code in huffman_codes.items():
            if code in encoded_text[shift:len(code) + shift]:
                decoded_text += val
                shift += len(code)

    return decoded_text


# расчет коэффициента сжатия
def calculateKcomp(input_s, encoded_s):
    return len(input_s) * 6 / len(encoded_s)


# расчет дисперсии
def calculateDispersion(freqs, shannonCodes, n):
    dispersion = 0

    # рассчитаем Lср
    l_middle = 0
    for f in freqs:
        # print(shannonCodes[f], len(freqs))
        l_middle += freqs[f]/n * len(shannonCodes[f])
    # l_middle = 4.5

    # l_middle = sum([freqs[x] * len(shannonCodes[x]) for x in freqs])

    for f in freqs:
        dispersion +=  freqs[f]/n  * ((len(shannonCodes[f]) - l_middle) ** 2)

    return dispersion


# Пример использования
text = input("Введите текст: ")
encoded_text, huffman_codes, freq_map = huffman_encoding(text)
print("Закодированный текст:", encoded_text, "Длинна:", len(text) * 5, "->", len(encoded_text))
print("Коды Хаффмана:", huffman_codes)
decoded_text = huffman_decoding(encoded_text, huffman_codes)
print("Раскодированный текст:", decoded_text)

print(f"\nКоэффициент сжатия: {calculateKcomp(text, encoded_text)}")
print(f"\nДисперсия: {calculateDispersion(freq_map, huffman_codes, len(text))}")
# в чащах юга жил бы цитрус? да но фальшивый экземпляр!
# victoria nulla est, Quam quae confessos animo quoque subjugat hostes