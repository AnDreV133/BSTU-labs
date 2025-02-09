//
// Created by admin on 28.01.2023.
//

#include <malloc.h>
#include <unistd.h>
#include "graficOfSinByMemory.h"

void HEHEHEHEHE_(int n) {
    if (n < 1) {
        sleep(1);
        return;
    }

    int *p = (int *) malloc(1024*1024*sizeof(int));

    for (int j = 0; j < 1024; j++) {
        p[j * 1024 + 100] = 42;
    }

    HEHEHEHEHE_(--n);

    free(p);
}

void HEHEHEHEHE() {
    for (int i = 0; i < 300; ++i) {
        HEHEHEHEHE_(400);
        sleep(1);
    }
}