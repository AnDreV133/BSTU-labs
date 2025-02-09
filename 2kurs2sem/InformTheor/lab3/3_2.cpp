#include "3_2.h"

std::vector<character> theShannonFanoMethod_(std::vector<character> a, int from, int to)
{
    if (to - from > 1)
    {
        int mid;
        if (to - from == 2)
            mid = from + 1;

        else
        {
            int sum = 0;
            for (int i = from; i < to; ++i)
                sum += a[i].numbers;
            int halfOfSum = sum / 2;
            sum = 0;
            int i = from;
            while (sum < halfOfSum && i < to)
            {
                sum += a[i].numbers;
                i++;
            }
            int k1 = abs(sum - a[i].numbers - halfOfSum);
            int k2 = abs(sum - halfOfSum);

            if (k1 < k2)
                mid = i;
            else
                mid = i + 1;
        }

        for (int i = from; i < mid; i++)
            a[i].code.push_back(0);
        a = theShannonFanoMethod_(a, from, mid);

        for (int i = mid; i < to; i++)
            a[i].code.push_back(1);
        a = theShannonFanoMethod_(a, mid, to);
    }

    return a;
}

// метод Шеннона-Фано
// возвращает таблицу, содержащую символ, его количество повторений, код
std::vector<character> theShannonFanoMethod(const std::vector<int> &table)
{
    std::vector<character> res = getTable(table);
    res = theShannonFanoMethod_(res, 0, res.size());

    return res;
}

int main___()
{
    std::string s;
    std::ifstream f0("T:\\2kurs2sem\\InformTheor\\lab3\\TI_3.txt");
    f0 >> s;

    f0.close();

    int length = 8;
    std::vector<int> r = getSequencesOfNCharactersEach(s, length);
    std::vector<character> res = theShannonFanoMethod(r);
    outputSymbolCodes(res, length);
    return 0;
}
