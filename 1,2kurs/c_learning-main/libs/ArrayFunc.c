//
// Created by admin on 09.12.2022.
//

#include <stdio.h>
#include <limits.h>

#include "ArrayFunc.h"

void inputIntArr(int *const a, const size_t n) {
    for (size_t i = 0; i < n; ++i)
        scanf("%d", &a[i]);
}


void outputIntArr(const int *const a, const size_t n) {
    printf("[");
    for (size_t i = 0; i < n; ++i)
        if (i != n - 1)
            printf("%d, ", a[i]);
        else
            printf("%d", a[i]);
    printf("]\n");
}

int sign(int x) {
    return x == 0 ? 0 : (x > 0 ? 1 : -1);
}

// Меняет значения переменных a и b между собой
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int getSignAbsNum(long long *x) {
    *x = *x > 0 ? *x : -*x;
    return sign(*x);
}

// Поиск позиции элемента в массиве a размера n со значением x с начала массива
size_t getIndexOfElement(const int *const a, const size_t n, int x) {
    size_t i = 0;
    while (i < n && a[i] != x)
        i++;

    return i;
}

// Поиск позиции первого отрицательного элемента в массиве a размера n
size_t getIndexOfFirstNegativeElement
(const int *const a, const size_t n) {
    size_t i = 0;
    while (i < n && a[i] >= 0)
        i++;

    return i;
}

// Поиск позиции последнего чётного элемента в массиве a размера n
size_t getIndexOfLastEvenElement(const int *const a, const size_t n) {
    size_t i = n;

    while (i != 0 && a[i - 1] % 2)
        i--;

    return i != 0 ? i - 1 : n;
}

// Вычисляет количество отрицательных элементов в массиве a размера n
unsigned int getAmountOfNegativeNumber(const int *const a, const size_t n) {
    unsigned int count = 0;
    for (size_t i = 0; i < n; ++i)
        if (a[i] < 0)
            count++;

    return count;
}


// Измененяет порядок элементов массива a размера n на обратный
void reverseArray(int *const a, const size_t n) {
    for (size_t i = 0; i < n / 2; ++i)
        swap(&a[i], &a[n - i - 1]);
}

// Сортирует массив a размера n выбором.
void sortBySelection(int *const a, const size_t n) {
    int *minNum = &a[0];
    for (int i = 1; i < n; ++i) {
        for (int j = i; j < n; ++j)
            if (a[j] < *minNum)
                minNum = &a[j];

        swap(minNum, &a[i - 1]);

        minNum = &a[i];
    }
}

// Вставляет элемент x по индексу pos в массив a размера n с сохранением относительного порядка других элементов
void addByIndex(int *const a, size_t *const n, size_t pos, int x) {
    if (*n >= pos) {
        for (size_t i = *n; i >= pos + 1; --i)
            a[i] = a[i - 1];
        a[pos] = x;

        (*n)++;
    }
}

// Добавляет элемент x в конец массива a размера n
void addToEnd(int *const a, size_t *const n, int x) {
    a[*n] = x;
    (*n)++;
}

// Удаляет элемент по индексу pos массива a размера n
// c сохранением относительного порядка других элементов
void delByIndex(int *const a, size_t *const n, size_t pos) {
    if (*n > pos) {
        for (size_t i = pos; i < *n; ++i) {
            a[i] = a[i + 1];
        }

        (*n)--;
    }
}

// копирует элементы массива arr размера lenArr в массив copyArr
void copyArray(const int *const arr, const size_t lenArr,
               int *const copyArr) {
    for (size_t i = 0; i < lenArr; ++i)
        copyArr[i] = arr[i];
}

// применяет функцию func(int) к элементам массива arr размера lenArr
void forEach(int *const arr, const size_t lenArr, int (*func)(int)) {
    for (size_t i = 0; i < lenArr; ++i)
        arr[i] = func(arr[i]);
}

// возвращает сумму элементов массива arr размера lenArr
long long getSumElemArray(const int *const arr, const size_t lenArr) {
    long long sum = 0;
    for (size_t i = 0; i < lenArr; ++i)
        sum += arr[i];

    return sum;
}

int getMinRecursive_(const int *const arr, const size_t lenArr, int minElem) {
    if (lenArr > 0)
        return getMinRecursive_(arr, lenArr - 1, minElem < arr[lenArr - 1] ? minElem : arr[lenArr - 1]);
    else
        return minElem;
}

int getMinRecursive(const int *const arr, const size_t lenArr) {
    return getMinRecursive_(arr, lenArr - 1, arr[lenArr - 1]);
}

int isValueInArray(int x, const int *arr, int n) {
    int isElemEqual = 0;
    for (int i = 0; i < n; ++i) {
        if (x == arr[i]) {
            isElemEqual = 1;
            break;
        }
    }

    return isElemEqual;
}