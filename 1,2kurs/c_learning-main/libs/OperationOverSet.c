//
// Created by admin on 01.02.2023.
//

#include <stdio.h>
#include "OperationOverSet.h"

// добавляет весь 1ый массив;
// добавляет элементы 2ого
// массива если не находит
// совпадений с 1ым
void mergeForUnsortSet(const int *arr1, int n1,
                       const int *arr2, int n2,
                       int *arrRes, int *nRes) {
    for (int i = 0; i < n1; ++i) {
        arrRes[i] = arr1[i];
    }

    int iRes = n1;
    for (int i2 = 0; i2 < n2; ++i2) {
        int isElemEqual = 0;
        for (int i1 = 0; i1 < n1; ++i1) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }

        if (!isElemEqual)
            arrRes[iRes++] = arr2[i2];
    }

    *nRes = iRes;
}

// добавляет элементы 1ого массива
// если не находит совпадений со 2ым
void differenceForUnsortSet(const int *arr1, int n1,
                            const int *arr2, int n2,
                            int *arrRes, int *nRes) {
    int iRes = 0;
    for (int i1 = 0; i1 < n1; ++i1) {
        int isElemEqual = 0;
        for (int i2 = 0; i2 < n2; ++i2) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }

        if (!isElemEqual)
            arrRes[iRes++] = arr1[i1];
    }

    *nRes = iRes;
}

// добавляет элемент если находит одинаковый элемент
void intersectionForUnsortSet(const int *arr1, int n1,
                              const int *arr2, int n2,
                              int *arrRes, int *nRes) {
    int iRes = 0;
    for (int i1 = 0; i1 < n1; ++i1) {
        int isElemEqual = 0;
        for (int i2 = 0; i2 < n2; ++i2) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }

        if (isElemEqual)
            arrRes[iRes++] = arr1[i1];
    }

    *nRes = iRes;
}

// дважды выполняет описанную ранее
// разность (1го над 2ым и 2го над 1ым)
void symmetricalDifferenceForUnsortSet(const int *arr1, int n1,
                                       const int *arr2, int n2,
                                       int *arrRes, int *nRes) {
    int iRes = 0;
    for (int i1 = 0; i1 < n1; ++i1) {
        int isElemEqual = 0;
        for (int i2 = 0; i2 < n2; ++i2) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }

        if (!isElemEqual)
            arrRes[iRes++] = arr1[i1];
    }

    for (int i2 = 0; i2 < n2; ++i2) {
        int isElemEqual = 0;
        for (int i1 = 0; i1 < n1; ++i1) {
            if (arr2[i2] == arr1[i1]) {
                isElemEqual = 1;
                break;
            }
        }

        if (!isElemEqual)
            arrRes[iRes++] = arr2[i2];
    }

    *nRes = iRes;
}

// выполняет разность универсума над множеством
void additionForUnsortSet(const int *arr, int n,
                          int *arrRes, int *nRes,
                          int sizeUniverse) {
    int iRes = 0;
    for (int iUniv = 1; iUniv <= sizeUniverse; ++iUniv) {
        int isElemEqual = 0;
        for (int i = 0; i < n; ++i) {
            if (iUniv == arr[i]) {
                isElemEqual = 1;
                break;
            }
        }

        if (!isElemEqual)
            arrRes[iRes++] = iUniv;
    }

    *nRes = iRes;
}

// перебирает элементы большего массива
// и сравнивает их с элементами меньшго
// массива, насчитывая количество совпадений
int isStrictInclusionForUnsortSet(const int *arr1, int n1,
                                  const int *arr2, int n2) {
    // размер 2го массива должен быть строго
    // больше 1го
    if (n2 <= n1)
        return 0;

    int countElemEqual = 0;
    for (int i1 = 0; i1 < n1; ++i1) {
        for (int i2 = 0; i2 < n2; ++i2)
            if (arr1[i1] == arr2[i2]) {
                countElemEqual++;
                break;
            }
        // преждевременно завершит ф-ию, если
        // все элементы 1го массива будет во 2ом
        if (countElemEqual == n1)
            return 1;
    }

    return 0;
}

// отличие от строго включения лишь в первом условии
int isNonStrictInclusionForUnsortSet(const int *arr1, int n1,
                                     const int *arr2, int n2) {
    // размер 2го массива должен быть больше либо
    // равен 1му
    if (n2 < n1)
        return 0;

    int countElemEqual = 0;
    for (int i2 = 0; i2 < n1; ++i2) {
        for (int i1 = 0; i1 < n1; ++i1)
            if (arr1[i1] == arr2[i2]) {
                countElemEqual++;
                break;
            }

        if (countElemEqual == n1)
            return 1;
    }

    return 0;
}

// изначально смотрим на первые элементы
// массивов и в зависимости от того какой
// элемент оказался меньше - сдвигаем указатель,
// также фиксируем равенство элементов
// при равенстве элементов записываем только
// один из равных и сдвигаем оба указателя
void mergeForSortSet(const int *arr1, int n1,
                     const int *arr2, int n2,
                     int *arrRes, int *nRes) {
    int iRes = 0;
    int i1 = 0, i2 = 0;
    while (i1 < n1 && i2 < n2)
        if (arr1[i1] == arr2[i2]) {
            arrRes[iRes++] = arr1[i1++];
            i2++;
        } else if (arr1[i1] < arr2[i2])
            arrRes[iRes++] = arr1[i1++];
        else
            arrRes[iRes++] = arr2[i2++];

    while (i2 < n2)
        arrRes[iRes++] = arr2[i2++];
    while (i1 < n1)
        arrRes[iRes++] = arr1[i1++];

    *nRes = iRes;
}

// равные элементы и элементы из второго массива
// игнорируем
void differenceForSortSet(const int *arr1, int n1,
                          const int *arr2, int n2,
                          int *arrRes, int *nRes) {
    int iRes = 0;
    int i1 = 0, i2 = 0;
    while (i1 < n1 && i2 < n2)
        if (arr1[i1] == arr2[i2]) {
            i1++;
            i2++;
        } else if (arr1[i1] < arr2[i2])
            arrRes[iRes++] = arr1[i1++];
        else
            i2++;

    while (i1 < n1)
        arrRes[iRes++] = arr1[i1++];

    *nRes = iRes;
}

// элементы из первого и второго массива
// игнорируем, кроме равных
void intersectionForSortSet(const int *arr1, int n1,
                            const int *arr2, int n2,
                            int *arrRes, int *nRes) {
    int iRes = 0;
    int i1 = 0, i2 = 0;
    while (i1 < n1 && i2 < n2)
        if (arr1[i1] == arr2[i2]) {
            arrRes[iRes++] = arr1[i1++];
            i2++;
        } else if (arr1[i1] < arr2[i2])
            i1++;
        else
            i2++;

    *nRes = iRes;
}

// равные элементы исключаем
void symmetricalDifferenceForSortSet(const int *arr1, int n1,
                                     const int *arr2, int n2,
                                     int *arrRes, int *nRes) {
    int iRes = 0;
    int i1 = 0, i2 = 0;
    while (i1 < n1 && i2 < n2)
        if (arr1[i1] == arr2[i2]) {
            i1++;
            i2++;
        } else if (arr1[i1] < arr2[i2])
            arrRes[iRes++] = arr1[i1++];
        else
            arrRes[iRes++] = arr2[i2++];

    while (i2 < n2)
        arrRes[iRes++] = arr2[i2++];
    while (i1 < n1)
        arrRes[iRes++] = arr1[i1++];

    *nRes = iRes;
}

// игнорируем равные с элементами универсума
void additionForSortSet(const int *arr, int n,
                        int *arrRes, int *nRes,
                        int sizeUniverse) {
    if (sizeUniverse < n)
        sizeUniverse = n + 1;

    int iRes = 0;
    int i = 0, iUniv = 0;
    while (i < n)
        if (arr[i] == iUniv + 1) {
            i++;
            iUniv++;
        } else
            arrRes[iRes++] = ++iUniv;

    while (iUniv < sizeUniverse)
        arrRes[iRes++] = ++iUniv;

    *nRes = iRes;
}

// проверяем размеры массивов и считаем количество
// равных элементов
int isStrictInclusionForSortSet(const int *arr1, int n1,
                                const int *arr2, int n2) {
    if (n2 <= n1)
        return 0;

    int countEqualElem = 0;
    int i1 = 0, i2 = 0;
    while (i1 < n1 && i2 < n2)
        if (arr1[i1] == arr2[i2]) {
            countEqualElem++;
            i1++;
            i2++;
        } else if (arr1[i1] < arr2[i2])
            i1++;
        else
            i2++;

    return countEqualElem == n1;
}

// также меняем условие для размеров
int isNonStrictInclusionForSortSet(const int *arr1, int n1,
                                   const int *arr2, int n2) {
    if (n2 < n1)
        return 0;

    int countEqualElem = 0;
    int i1 = 0, i2 = 0;
    while (i1 < n1 && i2 < n2)
        if (arr1[i1] == arr2[i2]) {
            countEqualElem++;
            i1++;
            i2++;
        } else if (arr1[i1] < arr2[i2])
            i1++;
        else
            i2++;

    return countEqualElem == n1;
}

void outputBoolArrStartOne(const int *a, int n) {
    printf("{ ");
    for (int i = 0; i < n; ++i)
        if (a[i])
            printf("%d ", i + 1);
    printf("}\n");
}

// пока индекс не дошел до универсума, в зависимости
// от условия, производим запись;
// проверяем если 0 и 0, то
// в итоговый массив записываем 0, иначе 1
void mergeForBoolSet(const int *arr1, const int *arr2,
                     int *arrRes, int sizeUniverse) {
    for (int iRes = 0; iRes < sizeUniverse; ++iRes)
        arrRes[iRes] = arr1[iRes] || arr2[iRes];
}

// если 1 и 0, то записываем 1, иначе 0
void differenceForBoolSet(const int *arr1, const int *arr2,
                          int *arrRes, int sizeUniverse) {
    for (int iRes = 0; iRes < sizeUniverse; ++iRes)
        arrRes[iRes] = arr1[iRes] > arr2[iRes];
}

// если 1 и 1, записываем 1, иначе 0
void intersectionForBoolSet(const int *arr1, const int *arr2,
                            int *arrRes, int sizeUniverse) {
    for (int iRes = 0; iRes < sizeUniverse; ++iRes)
        arrRes[iRes] = arr1[iRes] && arr2[iRes];
}

// если 1 и 1, записываем 0, иначе 1
void symmetricalDifferenceForBoolSet(const int *arr1, const int *arr2,
                                     int *arrRes, int sizeUniverse) {
    for (int iRes = 0; iRes < sizeUniverse; ++iRes)
        arrRes[iRes] = arr1[iRes] != arr2[iRes];
}

// записываем обратные значения (1->0, 0->1)
void additionForBoolSet(const int *arr, int *arrRes, int sizeUniverse) {
    for (int iRes = 0; iRes < sizeUniverse; ++iRes)
        arrRes[iRes] = !arr[iRes];
}

// Нам удовлетворяют случаи 1-0, 1-1 и 0-0, иначе
// false, также надо фиксировать, есть ли в первом
// массиве ещё элементы
int isStrictInclusionForBoolSet(const int *arr1, const int *arr2,
                                int sizeUniverse) {
    int haveOtherElem = 0;
    for (int i = 0; i < sizeUniverse; ++i) {
        if (arr1[i] < arr2[i])
            return 0;
        if (arr1[i] > arr2[i])
            haveOtherElem = 1;
    }

    return haveOtherElem;
}

// аналогично пред. ф-ии, но без флажка
int isNonStrictInclusionForBoolSet(const int *arr1, const int *arr2,
                                   int sizeUniverse) {
    for (int i = 0; i < sizeUniverse; ++i)
        if (arr1[i] < arr2[i])
            return 0;

    return 1;
}
