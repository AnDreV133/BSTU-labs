#include "5.h"


double getCompressionRatio(std::string s, const std::vector<character> &table)
{
    int numberOfSymbols = s.size();
    int b = numberOfSymbols;

    int b0 = 0;
    for (auto &x : table)
        b0 += x.numbers * x.code.size();
    double res = (double)b / b0;
    return res;
}

int main______()
{
    std::string s;
    std::ifstream f0("T:\\2kurs2sem\\InformTheor\\lab3\\TI_3.txt");
    f0 >> s;
    f0.close();

    int length = 8;
    std::vector<int> r = getSequencesOfNCharactersEach(s, length);

    clock_t start_time = clock();
    std::vector<character> res = theShannonFanoMethod(r);
    clock_t end_time = clock();
    clock_t work_time = end_time - start_time;

    std::cout << "Time: " << (double)work_time;
    std::cout << "\nCompression ratio: " << getCompressionRatio(s, res);

    return 0;
}
