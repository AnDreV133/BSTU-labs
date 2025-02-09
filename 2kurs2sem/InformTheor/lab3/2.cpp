#include "2.h"

int getNumberFromCharVector(const std::vector<char> &a)
{
    int i = a.size() - 1;
    int number = 0;
    for (auto &x : a)
    {
        if (x == '1')
            number += pow(2, i);
        i--;
    }
    return number;
}

std::vector<int> getSequencesOfNCharactersEach(const std::string &s, const int n)
{
    std::vector<int> sequences;
    std::vector<char> a(n);
    int i = 1;
    for (auto &x : s)
    {
        a[i] = x;
        if ((i + 1) % n == 0)
        {
            int p = getNumberFromCharVector(a);
            sequences.push_back(p);
            i = 0;
        }
        else
            i++;
    }

    return sequences;
}

void outputVector(const std::vector<int> &r)
{
    for (auto &x : r)
        std::cout << x << ' ';
    std::cout << '\n';
}

std::vector<int> getBinaryNumberNotation(std::vector<int> &a, int i, int n)
{
    int digit = n & 1;
    if (n == 0)
        return a;
    else
    {
        getBinaryNumberNotation(a, i + 1, n >> 1);
        a[i] = digit;
    }
    return a;
}

void outputVectorSequencesForReading(const std::vector<int> &r, const int &length)
{
    for (auto &x : r)
    {
        std::vector<int> a(length, 0);
        a = getBinaryNumberNotation(a, 0, x);
        for (auto &y : a)
        {
            std::cout << y;
        }
        std::cout << ' ';
    }
    std::cout << '\n';
}

int main_()
{
    std::string s;
    std::ifstream f0("T:\\2kurs2sem\\InformTheor\\lab3\\TI_3.txt");
    f0 >> s;
    f0.close();

    int length = 8;
    std::vector<int> r = getSequencesOfNCharactersEach(s, length);
    std::cout << "In computer memory:\n";

    outputVector(r);

    std::cout << "\nFor reading:\n";
    outputVectorSequencesForReading(r, length);

    return 0;
}
