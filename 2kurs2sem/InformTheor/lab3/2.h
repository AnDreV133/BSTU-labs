#ifndef num2
#define num2

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

void outputVectorSequencesForReading(const std::vector<int> &r, const int &length);
std::vector<int> getBinaryNumberNotation(std::vector<int> &a, int i, int n);
void outputVector(const std::vector<int> &r);
std::vector<int> getSequencesOfNCharactersEach(const std::string &s, const int n);
int getNumberFromCharVector(const std::vector<char> &a);

#endif