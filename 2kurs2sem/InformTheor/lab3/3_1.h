#ifndef num3_1
#define num3_1

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "2.h"

class character
{
public:
    std::vector<int> symbol;
    int numbers;
    std::vector<int> code;

    character()
    {
        symbol = std::vector<int>();
        code = std::vector<int>();
    }

    character(std::vector<int> symbol, int numbers, std::vector<int> code)
    {
        this->symbol = symbol;
        this->numbers = numbers;
        this->code = code;
    }
};

void outputSymbolCodes(const std::vector<character> &a, int codeLength);
std::vector<character> theHuffmanMethod(const std::vector<int> &table);
void reverseVector(std::vector<int> &v);
std::vector<character> getTable(const std::vector<int> &table);
int getSymbolPosition(const std::vector<character> &a, const int n);
// std::vector<int> getBinaryNumberNotation(std::vector<int> &a, int i, int n);

#endif
