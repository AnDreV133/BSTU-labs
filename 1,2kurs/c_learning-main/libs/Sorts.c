//
// Created by dmitr on 22.03.2023.
//

#include "Sorts.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

//int binarySearchLeft(const int *a, unsigned long long n) {
//    int left = 0;
//    int right = n;
//    while (right - left > 1) {
//        int middle = left + (right - left) / 2;
//        if (a[middle] <= 22)
//            left = middle;
//        else
//            right = middle;
//    }
//
//    return left;
//}

double f1(double x) {
    return (x + 1) * (x + 1);
}

double tripleSearch(double (*f)(double)) {
    double left = -7;
    double right = 10;
    double eps = 1e-6;
    while (right - left > eps) {
        double m1 = left + (right - left) / 3;
        double m2 = left + 2 * (right - left) / 3;
        if (f(m1) < f(m2))
            right = m2;
        else
            left = m1;
    }

    return right;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int isOrdered(int *a, unsigned long long n) {
    for (int i = 0; i < n - 1; ++i)
        if (a[i] > a[i + 1])
            return false;

    return true;
}

void shuffleArray(int *a, unsigned long long n) {
    srand(time(NULL));

    for (int i = 0; i < n; ++i) {
        int j = rand() % n;

        swap(&a[i], &a[j]);
    }
}

void bogoSort(int *a, unsigned long long n) {
    while (!isOrdered(a, n))
        shuffleArray(a, n);
}

void outputBin(int x) {
    for (int i = sizeof(x) * 5 - 1; i >= 0; --i) {
        printf("%d", x & (1 << i) ? 1 : 0);
    }
//    printf("\n");
}

void outputArray(int *a, unsigned long long n) {
    for (int i = 0; i < n; ++i) {
        printf("%d: %d ", i, a[i]);
//        printf("%d - ", i);
//        outputBin(a[i]);
//        printf(" ");
    }
    printf("\n");
}

void selectionSort(int *a, unsigned long long n) {
    for (int i = 0; i < n; ++i) {
        int minPos = i;
        for (int j = i + 1; j < n; ++j)
            if (a[j] < a[minPos])
                minPos = j;

        swap(&a[i], &a[minPos]);
    }
}

void insertionSort(int *a, unsigned long long n) {
    for (int i = 1; i < n; ++i) {
        int elem = a[i];
        int j = i;
        while (j > 0 && elem < a[j - 1]) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = elem;
    }
}

void bubbleSort(int *a, unsigned long long n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (a[j] > a[j + 1])
                swap(&a[j], &a[j + 1]);
}

void combSort(int *a, unsigned long long size) {
    size_t step = size;
    int swapped = 1;
    while (step > 1 || swapped) {
        if (step > 1)
            step /= 1.24733;
        swapped = 0;
        for (size_t i = 0, j = i + step; j < size; ++i, ++j)
            if (a[i] > a[j]) {
                swap(&a[i], &a[j]);
                swapped = 1;
            }
    }
}

void setMinMax(const int *a, unsigned long long n, int *min, int *max) {
    *min = a[0];
    *max = a[0];
    for (int i = 1; i < n; ++i)
        if (a[i] < *min)
            *min = a[i];
        else if (a[i] > *max)
            *max = a[i];
}

void countSort(int *a, unsigned long long n) {
    int min, max;
    setMinMax(a, n, &min, &max);

    int lenB = max - min + 1;
    int *b = calloc(lenB, sizeof(int));
    for (int i = 0; i < n; ++i) {
        b[a[i] - min]++;
    }

    int j = 0;
    for (int i = 0; i < lenB; ++i) {
        if (b[i] != 0)
            for (int jCopy = j; j < b[i] + jCopy; ++j)
                a[j] = i + min;
    }

    free(b);
}

int getValBy8Bit(int x, int i) {
    return 255 & ((x & (255 << (i * 8))) >> (i * 8));
}

void digitalSort(int *a, unsigned long long n) {
    for (int i = 0; i < sizeof(int); i++) {
        // массив для копирования
        int *new_a = malloc(n * sizeof(int));

        // массив для подсчёта "цифр"
        int *count_array = calloc(256 + 1, sizeof(int));

        // подсчёт и запись цифр в массив для подсчёта
        for (int j = 0; j < n; j++)
            count_array[getValBy8Bit(a[j], i)]++;

        // делаем из массива подсчётов массив сумм
        for (int j = 1; j < 256 + 1; j++) {
            count_array[j] += count_array[j - 1];
        }

        // заполняем новый массив в соответствии с "цифрами"
        for (int j = n - 1; j >= 0; j--) {
            int num = getValBy8Bit(a[j], i);
            new_a[count_array[num] - 1] = a[j];
            count_array[num]--;
        }

        // обновляем входной массив
        memcpy(a, new_a, n * sizeof(int));

        free(new_a);
        free(count_array);
    }

    unsigned long long j = 0;
    while (j < n && a[j] >= 0) j++;

    int *newA = malloc(n * sizeof(int));
    memcpy(newA, a + j, (n - j) * sizeof(int));

    memcpy(a + n - j, a, j * sizeof(int));
    memcpy(a, newA, (n - j) * sizeof(int));

    free(newA);
}

void ShellsSort(int *a, unsigned long long int n) {
    int step = n / 2;
    while (step) {
        for (int i = step; i < n; i++) {
            int iCopy = i;
            while (iCopy - step >= 0 && a[iCopy] < a[iCopy - step]) {
                swap(&a[iCopy], &a[iCopy - step]);
                iCopy -= step;
            }
        }

        step /= 2;
    }
}

int min(int a, int b) {
    return a < b ? a : b;
}

void merge(int *a, unsigned long long nA, int *b, unsigned long long nB, int *resArr) {
    int iterNA = 0;
    int iterNB = 0;
    for (int i = 0; i < nA + nB; ++i)
        if (iterNB == nB || iterNA < nA && a[iterNA] <= b[iterNB])
            resArr[i] = a[iterNA++];
        else
            resArr[i] = b[iterNB++];
}

void mergeSort_(int *a, int l, int r, int *b) {
    unsigned long long n = r - l;
    if (n <= 1) return;

    int mid = l + (r - l) / 2;
    mergeSort_(a, l, mid, b);
    mergeSort_(a, mid, r, b);

    merge(a + l, mid - l, a + mid, r - mid, b);

    memcpy(a + l, b, sizeof(int) * n);
}

void mergeSort(int *a, unsigned long long n) {
    int *b = (int *) malloc(n * sizeof(int));
    mergeSort_(a, 0, n, b);

    free(b);
}

int getMaxHeap(int *a) {
    return a[0];
}

int getParentPos(int i) {
    return (i - 1) / 2;
}

void insertHeap(int *a, int *n, int x) {
    a[(*n)++] = x;
    int index = *n - 1;
    int indexParent = getParentPos(index);
    while (a[index] > a[indexParent]) {
        swap(&a[index], &a[indexParent]);
        index = indexParent;
        indexParent = getParentPos(index);
    }
}

bool haveChild(int parentIndex, unsigned long long n) {
    return 2 * parentIndex + 1 < n;
}

bool haveChildRight(int parentIndex, unsigned long long n) {
    return 2 * parentIndex + 2 < n;
}

int getMaxIndexChild(int *a, unsigned long long n, int i) {
    int indexChildLeft = 2 * i + 1;
    int indexChildRight = indexChildLeft + 1;
    if (haveChildRight(i, n))
        if (a[indexChildLeft] > a[i] && a[indexChildLeft] > a[indexChildRight])
            return indexChildLeft;
        else if (a[indexChildRight] > a[i] && a[indexChildLeft] < a[indexChildRight]
                 && indexChildRight != n)
            return indexChildRight;
        else
            return n;
    else
        return a[indexChildLeft] > a[i] ? indexChildLeft : n;
}

void delMinHeap(int *a, int *n) {
    swap(&a[0], &a[--(*n)]);
    int index = 0;
    while (haveChild(index, *n)) {
        int curIndex = getMaxIndexChild(a, *n, index);
        if (curIndex != *n)
            swap(&a[index], &a[curIndex]);
        else
            break;

        index = curIndex;
    }
}

void heapSort(int *a, unsigned long long n) {
    int sizeB = 0;
    while (sizeB != n)
        insertHeap(a, &sizeB, a[sizeB]);
    outputArray(a, sizeB);
    while (sizeB) {
        delMinHeap(a, &sizeB);
        outputArray(a, sizeB);
    }
}

int rolePosArr(int l, int r) {
    srand(time(NULL));

    return l + rand() % (r - l);
}

void quickSort_(int a[], int l, int r) {
    if (r - l <= 1)
        return;
    else {
        int begin = l;
        int end = r;
        int x = a[l + (r - l) / 2];
        while (r > l) {
            while (a[l] < x) l++;
            while (a[r] > x) r--;
            if (l < r) swap(&a[l++], &a[r--]);
        }

        quickSort_(a, begin, l);
        quickSort_(a, r, end);
    }
}

void quickSort(int a[], unsigned long long n) {
    quickSort_(a, 0, n - 1);
}

unsigned long long combSortInjected(int *a, unsigned long long int size) {
    unsigned long long countOperations = 0;

    size_t step = size;
    int swapped = 1;
    while (step > 1 || swapped) {
        countOperations++;
        if (step > 1)
            step /= 1.24733;

        swapped = 0;
        for (size_t i = 0, j = i + step; j < size; ++i, ++j) {
            countOperations++;
            if (a[i] > a[j]) {
                swap(&a[i], &a[j]);
                swapped = 1;
            }
        }
    }

    return countOperations;
}

unsigned long long bubbleSortInjected(int *a, unsigned long long int n) {
    unsigned long long countOperations = 0;

    for (int i = 0; i < n - 1; ++i) {
        countOperations++;
        for (int j = 0; j < n - i - 1; ++j) {
            countOperations++;
            if (a[j] > a[j + 1])
                swap(&a[j], &a[j + 1]);
        }
    }

    return countOperations;
}

unsigned long long insertionSortInjected(int *a, unsigned long long int n) {
    unsigned long long countOperations = 0;

    for (int i = 1; i < n; ++i) {
        countOperations++;
        int elem = a[i];
        int j = i;
        while (j > 0 && elem < a[j - 1]) {
            countOperations++;
            a[j] = a[j - 1];
            j--;
        }

        a[j] = elem;
    }

    return countOperations;
}

unsigned long long selectionSortInjected(int *a, unsigned long long int n) {
    unsigned long long countOperations = 0;

    for (int i = 0; i < n; ++i) {
        countOperations++;
        int minPos = i;
        for (int j = i + 1; j < n; ++j) {
            countOperations++;
            if (a[j] < a[minPos]) {
                minPos = j;
            }
        }

        swap(&a[i], &a[minPos]);
    }

    return countOperations;
}

unsigned long long ShellsSortInjected(int *a, unsigned long long int n) {
    unsigned long long countOperations = 0;

    int step = n / 2;
    while (step) {
        countOperations++;
        for (int i = step; i < n; i++) {
            countOperations++;
            int iCopy = i;
            while (iCopy - step >= 0 && a[iCopy] < a[iCopy - step]) {
                countOperations++;
                swap(&a[iCopy], &a[iCopy - step]);
                iCopy -= step;
            }
        }

        step /= 2;
    }

    return countOperations;
}

unsigned long long digitalSortInjected(int *a, unsigned long long int n) {
    unsigned long long countOperations = 0;

    for (int i = 0; i < sizeof(int); i++) {
        countOperations++;
        // массив для копирования
        int *new_a = malloc(n * sizeof(int));

        // массив для подсчёта "цифр"
        int *count_array = calloc(256 + 1, sizeof(int));

        // подсчёт и запись цифр в массив для подсчёта
        for (int j = 0; j < n; j++) {
            countOperations++;
            count_array[getValBy8Bit(a[j], i)]++;
        }

        // делаем из массива подсчётов массив сумм
        for (int j = 1; j < 256 + 1; j++) {
            countOperations++;
            count_array[j] += count_array[j - 1];
        }

        // заполняем новый массив в соответствии с "цифрами"
        for (int j = n - 1; j >= 0; j--) {
            countOperations++;
            int num = getValBy8Bit(a[j], i);
            new_a[count_array[num] - 1] = a[j];
            count_array[num]--;
        }

        // обновляем входной массив
        memcpy(a, new_a, n * sizeof(int));

        free(new_a);
        free(count_array);
    }

    unsigned long long j = 0;
    while (j < n && a[j] >= 0) {
        countOperations++;
        j++;
    }

    int *newA = malloc(n * sizeof(int));
    memcpy(newA, a + j, (n - j) * sizeof(int));

    memcpy(a + n - j, a, j * sizeof(int));
    memcpy(a, newA, (n - j) * sizeof(int));

    free(newA);

    return countOperations;
}
