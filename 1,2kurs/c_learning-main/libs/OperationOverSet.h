//
// Created by admin on 01.02.2023.
//

#ifndef MAIN_C_OPERATIONOVERSET_H
#define MAIN_C_OPERATIONOVERSET_H

#endif //MAIN_C_OPERATIONOVERSET_H


void mergeForUnsortSet(const int *arr1, int n1,
                       const int *arr2, int n2,
                       int *arrRes, int *nRes);

void differenceForUnsortSet(const int *arr1, int n1,
                            const int *arr2, int n2,
                            int *arrRes, int *nRes);


void intersectionForUnsortSet(const int *arr1, int n1,
                              const int *arr2, int n2,
                              int *arrRes, int *nRes);

void symmetricalDifferenceForUnsortSet(const int *arr1, int n1,
                                       const int *arr2, int n2,
                                       int *arrRes, int *nRes);

void additionForUnsortSet(const int *arr, int n,
                          int *arrRes, int *nRes,
                          int sizeUniverse);

int isStrictInclusionForUnsortSet(const int *arr1, int n1,
                                  const int *arr2, int n2);

int isNonStrictInclusionForUnsortSet(const int *arr1, int n1,
                                     const int *arr2, int n2);

void mergeForSortSet(const int *arr1, int n1,
                     const int *arr2, int n2,
                     int *arrRes, int *nRes);

void differenceForSortSet(const int *arr1, int n1,
                          const int *arr2, int n2,
                          int *arrRes, int *nRes);


void intersectionForSortSet(const int *arr1, int n1,
                            const int *arr2, int n2,
                            int *arrRes, int *nRes);

void symmetricalDifferenceForSortSet(const int *arr1, int n1,
                                     const int *arr2, int n2,
                                     int *arrRes, int *nRes);

void additionForSortSet(const int *arr, int n,
                        int *arrRes, int *nRes,
                        int sizeUniverse);

int isStrictInclusionForSortSet(const int *arr1, int n1,
                                const int *arr2, int n2);

int isNonStrictInclusionForSortSet(const int *arr1, int n1,
                                   const int *arr2, int n2);

void mergeForBoolSet(const int *arr1, const int *arr2,
                     int *arrRes, int sizeUniverse);

void differenceForBoolSet(const int *arr1, const int *arr2,
                          int *arrRes, int sizeUniverse);

void intersectionForBoolSet(const int *arr1, const int *arr2,
                            int *arrRes, int sizeUniverse);

void symmetricalDifferenceForBoolSet(const int *arr1, const int *arr2,
                                     int *arrRes, int sizeUniverse);


void additionForBoolSet(const int *arr, int *arrRes,
                        int sizeUniverse);

int isStrictInclusionForBoolSet(const int *arr1,
                                const int *arr2,
                                int sizeUniverse);

int isNonStrictInclusionForBoolSet(const int *arr1,
                                   const int *arr2,
                                   int sizeUniverse);

void outputBoolArrStartOne(const int *a, int n);