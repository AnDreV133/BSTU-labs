#ifndef MAIN_C_OPERATIONOVERMATRIX_H
#define MAIN_C_OPERATIONOVERMATRIX_H

#endif //MAIN_C_OPERATIONOVERMATRIX_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <memory.h>
#include <math.h>

#include <stdbool.h>

typedef struct Matrix_ {
    int **val;
    int nRows;
    int nCols;
} Matrix;

typedef struct Position_ {
    int iRow;
    int iCol;
} Position;

void swapRows(Matrix m, int i1, int i2);

void swapCols(Matrix m, int j1, int j2);

void insertionSortRowsMatrixByRowCriteria(Matrix m, int (*criteria)(const int *, int));

void selectionSortRowsMatrixByRowCriteria(Matrix m, int (*criteria)(const int *, int));

void insertionSortRowsMatrixByRowCriteriaF(Matrix m, float (*criteria)(const int *, int));

void selectionSortColsMatrixByColCriteria(Matrix m, int (*criteria)(const int *, int));

bool isSquareMatrix(Matrix m);

bool areTwoMatricesEqual(Matrix m1, Matrix m2);

bool isEMatrix(Matrix m);

bool isSymmetricMatrix(Matrix m);

Matrix getMemMatrix(int nRows, int nCols);

Matrix *getMemArrayOfMatrices(int nMatrices,
                              int nRows, int nCols);

void freeMemMatrix(Matrix m);

void freeMemMatrices(Matrix *mArr, int nMatrices);

void inputMatrix(Matrix *m);

void inputMatrices(Matrix *mArr, int nMatrices);

void outputMatrix(Matrix m);

void outputMatrices(Matrix *mArr, int nMatrices);

void swapPointers(int **a, int **b);

Position getMinValPosMatrix(Matrix m);

Position getMaxValPosMatrix(Matrix m);

Matrix createMatrixFromArray(const int *a,
                             int nRows, int nCols);

Matrix *createArrayOfMatrixFromArray(const int *values, int nMatrices,
                                     int nRows, int nCols);

void transposeSquareMatrix(Matrix m);

void transposeMatrix(Matrix *m);

int countNonDescendingRowsMatrices(Matrix *ms, int nMatrix);

long long getNSpecialElement(Matrix m);

void sortByDistances(Matrix m);

long long getSpecialScalarProduct(Matrix m);

long long getSumOfMaxesOfPseudoDiagonal(Matrix m);

