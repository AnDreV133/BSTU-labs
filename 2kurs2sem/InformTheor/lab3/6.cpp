#include "6.h"

bool areVectorsEqual(std::vector<int> a, std::vector<int> b)
{
    if (a.size() != b.size())
        return false;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i])
            return false;
    return true;
}

int getPosOfTheVector(const std::vector<character> &table, const std::vector<int> &a)
{
    for (int i = 0; i < table.size(); i++)
        if (areVectorsEqual(table[i].code, a))
            return i;
    return -1;
}

std::string decoding(std::string codingS, std::vector<character> table, int length)
{
    std::string res;
    std::vector<int> a;
    for (auto &x : codingS)
    {
        a.push_back(x - '0');
        int pos = getPosOfTheVector(table, a);
        if (pos >= 0)
        {
            std::vector<int> b(length, 0);
            b = getBinaryNumberNotation(b, 0, table[pos].symbol[0]);
            reverseVector(b);
            for (auto &y : b)
                res.push_back(y + '0');
            a.clear();
        }
    }
    return res;
}

int main()
{
    std::string s;
    std::ifstream f0("T:\\2kurs2sem\\InformTheor\\lab3\\TI_3.txt");
    f0 >> s;
    f0.close();

    int length = 2;
    std::vector<int> r = getSequencesOfNCharactersEach(s, length);

    std::vector<character> res1 = theShannonFanoMethod(r);
    std::string sCode1 = replaceCharactersWithTheirCodes(res1, r);
    std::string s1 = decoding(sCode1, res1, length);

    std::vector<character> res2 = theHuffmanMethod(r);
    std::string sCode2 = replaceCharactersWithTheirCodes(res2, r);
    std::string s2 = decoding(sCode2, res2, length);
    if (s1 == s2)
        std::cout << "YES!!!\n";
    else
        std::cout << "NO!!\n";

    return 0;
}
