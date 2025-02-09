//
// Created by admin on 20.12.2022.
//

#ifndef MAIN_C_TRUTHTABLE_H
#define MAIN_C_TRUTHTABLE_H

#endif //MAIN_C_TRUTHTABLE_H

#include <stdlib.h>

#define bits unsigned int

bits tB(size_t n, bits b);

bits reverseBits(size_t size, bits b);

void outputTruthTable(unsigned char size, bits (*func)(bits));