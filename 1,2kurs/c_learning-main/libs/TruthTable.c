//
// Created by admin on 20.12.2022.
//

#include "TruthTable.h"

#include <stdio.h>

#define bits unsigned int

bits tB(size_t n, bits b) {
    return (b & (1 << n)) > 0 ? 1 : 0;
}

bits reverseBits(size_t size, bits b) {
    bits res = 0;
    for (size_t i = size; i > 0; --i, b >>= 1)
        res += (b & 1) << (i - 1);

    return res;
}

void outputTruthTable(unsigned char size, bits (*func)(bits)) {
    bits b = 0;
    for (size_t header = 0; header < size; ++header)
        printf("  x%zu", header);
    printf("  f\n");
    for (size_t row = 0; row < 1 << size; ++row, ++b) {// continue
        for (size_t column = 0; column < size; ++column)
            printf("| %u ", tB(size - column - 1, b));

        printf("| %u |\n", tB(0, func(reverseBits(size, b))));
    }
}

//bits neg(bits b) {
//    return !tB(0, b);
//}
//
//bits conjunc(bits b) {
//    return tB(0, b) & tB(1, b);
//}
//
//bits disjunc(bits b) {
//    return tB(0, b) | tB(1, b);
//}
//
//bits exclusOr(bits b) {
//    return !tB(0, b) & tB(1, b) | tB(0, b) & !tB(1, b);
//}
//
//bits equival(bits b) {
//    return tB(0, b) & tB(1, b) | !tB(0, b) & !tB(1, b);
//}
//
//bits implicat(bits b) {
//    return !tB(0, b) | tB(1, b);
//}
//
//bits pierceArrow(bits b) {
//    return !tB(0, b) & !tB(1, b);
//}
//
//bits schefferStroke(bits b) {
//    return !tB(0, b) | !tB(1, b);
//}
//
//bits funcG(bits b) {
//    return !tB(0, b) & !tB(1, b) & !tB(3, b) & !tB(4, b) |
//           !tB(0, b) & !tB(1, b) & !tB(2, b) & !tB(3, b) |
//           !tB(1, b) & !tB(2, b) & !tB(3, b) & !tB(4, b) |
//           !tB(0, b) & tB(1, b) & !tB(2, b) & tB(3, b) |
//           !tB(0, b) & tB(1, b) & tB(3, b) & !tB(4, b) |
//           tB(1, b) & !tB(2, b) & tB(3, b) & !tB(4, b) |
//           tB(0, b) & tB(1, b) & !tB(2, b) & !tB(3, b) & tB(4, b) |
//           tB(0, b) & !tB(1, b) & tB(2, b) & tB(3, b) & !tB(4, b) |
//           !tB(0, b) & !tB(2, b) & !tB(3, b) & !tB(4, b);
//}
//
//bits funcG1(bits b) {
//    return (tB(0, b) | tB(1, b) | tB(2, b) | !tB(3, b)) &
//           (tB(0, b) | tB(1, b) | !tB(2, b) | !tB(4, b)) &
//           (!tB(1, b) | !tB(2, b) | tB(3, b)) &
//           (!tB(1, b) | !tB(2, b) | !tB(3, b) | !tB(4, b)) &
//           (!tB(0, b) | tB(1, b) | tB(2, b) | !tB(4, b)) &
//           (!tB(0, b) | tB(1, b) | !tB(2, b) | tB(3, b)) &
//           (tB(0, b) | tB(1, b) | tB(2, b) | tB(3, b) | tB(4, b)) &
//           (tB(0, b) | !tB(1, b) | tB(2, b) | tB(3, b) | !tB(4, b)) &
//           (!tB(0, b) | tB(1, b) | tB(2, b) | !tB(3, b) | tB(4, b)) &
//           (!tB(0, b) | tB(1, b) | !tB(2, b) | !tB(3, b) | !tB(4, b)) &
//           (!tB(0, b) | !tB(1, b) | tB(2, b) | tB(3, b) | tB(4, b)) &
//           (!tB(0, b) | !tB(1, b) | tB(2, b) | !tB(3, b) | !tB(4, b)) &
//           (!tB(0, b) | !tB(1, b) | !tB(2, b) | !tB(3, b) | tB(4, b));
//}