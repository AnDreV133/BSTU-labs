#include "specificOperationOverSet.h"

void mergeWithoutDuplicateValues(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1 + n2;
    // записываем первый массив в итоговый
    for (int i = 0; i < n1; ++i) {
        arrRes[i] = arr1[i];
    }
    // записываем второй массив, но с условием
    int iRes = n1;
    for (int i2 = 0; i2 < n2; ++i2) {
        // проверяем на то имеется ли элемент второго массива в первом массиве
        int isElemEqual = 0;
        for (int i1 = 0; i1 < n1; ++i1) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }
        // записываем значение если не найдено совпадений, иначе уменьшаем размер на 1
        if (isElemEqual)
            (*nRes)--;
        else
            arrRes[iRes++] = arr2[i2];
    }
}

void mergeWithDuplicateValues(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1 + n2;
    // записываем первый массив
    for (int i1 = 0; i1 < n1; ++i1) {
        arrRes[i1] = arr1[i1];
    }
    // записываем второй массив
    for (int i2 = 0; i2 < n2; ++i2) {
        arrRes[i2 + n1] = arr2[i2];
    }
}

void intersectionFirstArrWithSecondArr(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1;
    // запись первого массива с исключением элементов
    int iRes = 0;
    for (int i1 = 0; i1 < n1; ++i1) {
        // определяем наличие равных элементов
        int isElemEqual = 0;
        for (int i2 = 0; i2 < n2; ++i2) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }
        // записываем значение если не найдено совпадений, иначе уменьшаем размер на 1
        if (isElemEqual)
            (*nRes)--;
        else
            arrRes[iRes++] = arr1[i1];
    }
}

void mergeWithoutEqualElemsArrays(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1 + n2;
    // запись первого массива с исключением элементов
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
            (*nRes)--;
        else
            arrRes[iRes++] = arr1[i1];
    }
    // запись второго массива с исключением элементов
    for (int i2 = 0; i2 < n2; ++i2) {
        int isElemEqual = 0;
        for (int i1 = 0; i1 < n1; ++i1) {
            if (arr2[i2] == arr1[i1]) {
                isElemEqual = 1;
                break;
            }
        }

        if (isElemEqual)
            (*nRes)--;
        else
            arrRes[iRes++] = arr2[i2];
    }
}

int isSecondArrHaveAllElemFirstArr(const int *arr1, int n1, const int *arr2, int n2) {
    // размер второго масива должен быть больше либо равен
    // размеру первого массива
    if (n1 > n2)
        return 0;
    // увеличиваем счётчик каждый раз когда находим совпадения
    int countEqualElem = 0;
    for (int i1 = 0; i1 < n1; ++i1) {
        for (int i2 = 0; i2 < n2; ++i2) {
            if (arr1[i1] == arr2[i2]) {
                countEqualElem++;
                break;
            }
        }
    }
    // если значение счётчика равно размеру меньшего массива
    // (первого), то значит все элементы меньшего массива
    // имеются в большем
    return countEqualElem == n1;
}

int isElemsArraysEquals(const int *arr1, int n1, const int *arr2, int n2) {
    // если размеры неравны, то массивы
    // наверняка неравны
    if (n1 != n2)
        return 0;
    // если встретится хотя бы один неравный элемент,
    // то функция выведет false
    for (int i1 = 0; i1 < n1; ++i1) {
        int isElemEqual = 0;
        for (int i2 = 0; i2 < n2; ++i2) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }

        if (!isElemEqual)
            return 0;
    }
    // после всех проверок можно сказать,
    // что массивы соответствуют условию
    return 1;
}

int isArraysDontHaveCommonElems(const int *arr1, int n1, const int *arr2, int n2) {
    // определяются равные элементы
    for (int i1 = 0; i1 < n1; ++i1) {
        int isElemEqual = 0;
        for (int i2 = 0; i2 < n2; ++i2) {
            if (arr1[i1] == arr2[i2]) {
                isElemEqual = 1;
                break;
            }
        }
        // если равны, то выводим false
        if (isElemEqual)
            return 0;
    }
    // если не найдено равных элементов, то true
    return 1;
}

void mergeWithSortWithoutDuplicateElemsArrays(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1 + n2;
    // также как и в предыдущем варианте
    for (int iRes = 0, i1 = 0, i2 = 0; iRes < *nRes; ++iRes) {
        // при равных элементах происходит единственная запись
        // и индекс в обоих массивах увеличмвается на 1
        if (i1 < n1 && i2 < n2 && arr1[i1] == arr2[i2]) {
            arrRes[iRes] = arr1[i1++];
            i2++;
            (*nRes)--;
        } else if (i2 == n2 || arr1[i1] < arr2[i2] && i1 < n1)
            arrRes[iRes] = arr1[i1++];
        else
            arrRes[iRes] = arr2[i2++];
    }
}

void mergeWithSortWithEqualElemsArrays(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1 + n2;
    // проходимся по массивам одновременно, если один элемент
    // меньше чем второй, то увеличиваем индекс массива, где
    // располагался первый элемент, и наоборот
    for (int iRes = 0, i1 = 0, i2 = 0; iRes < *nRes; ++iRes) {
        // также если индекс равен размеру, то оставшиееся элементы
        // последовательно выгружаются в итоговый массив
        if (i2 == n2 || arr1[i1] <= arr2[i2] && i1 < n1)
            arrRes[iRes] = arr1[i1++];
        else
            arrRes[iRes] = arr2[i2++];
    }
}

void mergeWithSortWithoutElemsSecondArr(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1 + n2;
    // производим запись только первого массива и исключаем
    // совпадения со вторым
    for (int iRes = 0, i1 = 0, i2 = 0; iRes < *nRes; ++iRes) {
        if (i1 < n1 && i2 < n2 && arr1[i1] == arr2[i2]) {
            i1++;
            i2++;
            (*nRes) -= 2;
            iRes--;
        } else if (i2 == n2 || arr1[i1] < arr2[i2] && i1 < n1)
            arrRes[iRes] = arr1[i1++];
            // тк за итерацию мы пропустили запись по iRes,
            // то iRes возвращаем обратно
        else {
            i2++;
            (*nRes)--;
            iRes--;
        }
    }
}

void mergeWithSortWithoutEqualElemsArrays(const int *arr1, int n1, const int *arr2, int n2, int *arrRes, int *nRes) {
    // задаём максимальный размер итогового массива
    *nRes = n1 + n2;
    // прозводим запись игнорируя равные элементы
    for (int iRes = 0, i1 = 0, i2 = 0; iRes < *nRes; ++iRes) {
        if (i1 < n1 && i2 < n2 && arr1[i1] == arr2[i2]) {
            i1++;
            i2++;
            (*nRes) -= 2;
            iRes--;
        } else if (i2 == n2 || arr1[i1] < arr2[i2] && i1 < n1)
            arrRes[iRes] = arr1[i1++];
        else {
            arrRes[iRes] = arr2[i2++];
        }
    }
}

int isSecondArrHaveAllElemFirstArrForSort(const int *arr1, int n1, const int *arr2, int n2) {
    // полносью проходимся по второму массиву,
    int countEqualElem = 0;
    for (int i2 = 0, i1 = 0; i2 < n2 && i1 < n1; ++i2) {
        // добавляем 1 к индексу каждый раз когда находим совпадение
        if (arr2[i2] == arr1[i1]) {
            countEqualElem++;
            i1++;
        }
    }
    // если все элементы соответсвуют, то счетчик равен длине
    // первого массива
    return countEqualElem == n1;
}