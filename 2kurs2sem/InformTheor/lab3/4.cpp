#include "4.h"

std::string
replaceCharactersWithTheirCodes(const std::vector<character> &table, const std::vector<int> &s)
{
    std::vector<char> a;
    std::string res;
    for (auto &x : s)
    {
        int pos = getSymbolPosition(table, x);
        if (pos >= 0)
            for (auto &y : table[pos].code)
                res.push_back('0' + y);
    }
    return res;
}

int main____()
{
    std::string s;

    std::ifstream f0("T:\\2kurs2sem\\InformTheor\\lab3\\TI_3.txt");
    f0 >> s;
    f0.close();

    int length = 8;
    std::vector<int> r = getSequencesOfNCharactersEach(s, length);
    std::vector<character> res = theShannonFanoMethod(r);
    std::string sCode = replaceCharactersWithTheirCodes(res, r);
    std::cout << sCode;
    return 0;
}
