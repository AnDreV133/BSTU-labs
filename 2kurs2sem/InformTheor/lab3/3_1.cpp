
#include "3_1.h"



// возвращает позицию элемента n в векторе а, если элемента нет в векторе, то возвращае т -1.
// позиция элемента n - позиция элемента в векторе а, полем symbol которого равно n
int getSymbolPosition(const std::vector<character> &a, const int n)
{
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < a[i].symbol.size(); j++)
            if (a[i].symbol.at(j) == n)
                return i;
    return -1;
}

bool comp(const character &a, const character &b)
{
    return a.numbers > b.numbers;
}

// возвращает отсортированный по неувозрастанию вектор содержащий
// структуру character, полученную после обработки вектора table,
// содержащего двоичные последовательности, записанные целыми числами
// поле symbol - двоичная последовательность, записанная целыми числами
// поле numbers - количество раз, сколько последовательность встречается
// в векторе table, поле code - пустое
std::vector<character> getTable(const std::vector<int> &table)
{
    std::vector<character> res;
    for (auto &x : table)
    {
        int pos = getSymbolPosition(res, x);
        if (pos == -1)
            res.push_back(character(std::vector<int>{x}, 1, std::vector<int>()));
        else
            res[pos].numbers++;
    }
    std::sort(res.begin(), res.end(), comp);

    return res;
}

void reverseVector(std::vector<int> &v)
{

    for (int i = 0; i < v.size() / 2; i++)
    {
        bool c = v[i];
        v[i] = v[v.size() - 1 - i];
        v[v.size() - 1 - i] = c;
    }
}

// метод Хаффмана
// возвращает таблицу, содержащую символ, его количество повторений, код
std::vector<character> theHuffmanMethod(const std::vector<int> &table)
{
    std::vector<character> res = getTable(table);
    std::vector<character> p = res;

    while (p.size() > 1)
    {
        int n = p.size() - 1;
        for (int i = 0; i < p[n].symbol.size(); i++)
        {
            int k = getSymbolPosition(res, p[n].symbol[i]);
            res[k].code.push_back(0);
        }

        n--;
        for (int i = 0; i < p[n].symbol.size(); i++)
        {
            res[getSymbolPosition(res, p[n].symbol[i])].code.push_back(1);
        }

        p[n].numbers += p[n + 1].numbers;
        for (int i = 0; i < p[n + 1].symbol.size(); i++)
            p[n].symbol.push_back(p[n + 1].symbol[i]);
        p.erase(p.cend());
        std::sort(p.begin(), p.end(), comp);
    }
    for (auto &x : res)
        reverseVector(x.code);

    return res;
}

void outputSymbolCodes(const std::vector<character> &a, int codeLength)
{
    for (character element : a)
    {
        std::cout << "Symbol: " << element.symbol[0] << " / ";
        std::vector<int> a(codeLength, 0);
        a = getBinaryNumberNotation(a, 0, element.symbol[0]);
        for (auto &x : a)
        {
            std::cout << x;
        }
        std::cout << "\nCode: ";

        for (int x : element.code)
        {
            std::cout << x;
        }
        std::cout << "\n\n";
    }
}

int main__()
{
    std::string s;
    std::ifstream f0("T:\\2kurs2sem\\InformTheor\\lab3\\TI_3.txt");
    f0 >> s;
    f0.close();

    int length = 8;
    std::vector<int> r = getSequencesOfNCharactersEach(s, length);
    std::vector<character> res = theHuffmanMethod(r);
    outputSymbolCodes(res, length);
    return 0;
}
