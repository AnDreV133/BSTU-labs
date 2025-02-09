//
// Created by admin on 01.02.2023.
//

#ifndef MAIN_C_SPECIFICOPERATIONOVERSET_H
#define MAIN_C_SPECIFICOPERATIONOVERSET_H

#endif //MAIN_C_SPECIFICOPERATIONOVERSET_H

void mergeWithoutDuplicateValues(const int arr1[], int n1,
                                 const int arr2[], int n2,
                                 int arrRes[], int *nRes);

void mergeWithDuplicateValues(const int arr1[], int n1,
                              const int arr2[], int n2,
                              int arrRes[], int *nRes);

void intersectionFirstArrWithSecondArr(const int arr1[], int n1,
                                       const int arr2[], int n2,
                                       int arrRes[], int *nRes);


void mergeWithoutEqualElemsArrays(const int arr1[], int n1,
                                  const int arr2[], int n2,
                                  int arrRes[], int *nRes);

int isSecondArrHaveAllElemFirstArr(const int arr1[], int n1,
                                   const int arr2[], int n2);


int isElemsArraysEquals(const int arr1[], int n1,
                        const int arr2[], int n2);

int isArraysDontHaveCommonElems(const int arr1[], int n1,
                                const int arr2[], int n2);

void mergeWithSortWithEqualElemsArrays(const int arr1[], int n1,
                                       const int arr2[], int n2,
                                       int arrRes[], int *nRes);


void mergeWithSortWithoutDuplicateElemsArrays(const int arr1[], int n1,
                                              const int arr2[], int n2,
                                              int arrRes[], int *nRes);

void mergeWithSortWithoutElemsSecondArr(const int arr1[], int n1,
                                        const int arr2[], int n2,
                                        int arrRes[], int *nRes);

void mergeWithSortWithoutEqualElemsArrays(const int arr1[], int n1,
                                          const int arr2[], int n2,
                                          int arrRes[], int *nRes);

int isSecondArrHaveAllElemFirstArrForSort(const int arr1[], int n1,
                                          const int arr2[], int n2);