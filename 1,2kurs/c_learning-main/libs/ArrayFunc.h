//
// Created by admin on 09.12.2022.
//

#ifndef MAIN_C_ARRAYFUNC_H
#define MAIN_C_ARRAYFUNC_H

#endif //MAIN_C_ARRAYFUNC_H

#include <stdio.h>

// Ввод массива a размера n
void inputIntArr(int *const a, const size_t n);

// Вывод массива a размера n
void outputIntArr(int const *const a, const size_t n);

int getMinRecursive(const int *const arr, const size_t lenArr);

void addToEnd(int *const a, size_t *const n, int x);

void sortBySelection(int *const a, const size_t n);

void reverseArray(int *const a, const size_t n);

int isValueInArray(int x, const int *arr, int n);