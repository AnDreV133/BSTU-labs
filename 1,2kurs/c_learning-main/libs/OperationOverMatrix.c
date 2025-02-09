

#include "OperationOverMatrix.h"

Matrix getMemMatrix(int nRows, int nCols) {
    assert(nRows > 0 && nCols > 0);

    int **a = (int **) malloc(sizeof(int *) * nRows);
    for (int i = 0; i < nRows; ++i)
        a[i] = (int *) malloc(sizeof(int) * nCols);

    return (Matrix) {a, nRows, nCols};
}

void freeMemMatrix(Matrix m) {
    for (int i = 0; i < m.nRows; ++i)
        free(m.val[i]);
    free(m.val);
}

Matrix *getMemArrayOfMatrices(int nMatrices, int nRows, int nCols) {
    Matrix *arr = (Matrix *) malloc(sizeof(Matrix) * nMatrices);
    for (int i = 0; i < nMatrices; ++i)
        arr[i] = getMemMatrix(nRows, nCols);

    return arr;
}

void freeMemMatrices(Matrix *mArr, int nMatrices) {
    for (int i = 0; i < nMatrices; ++i)
        freeMemMatrix(mArr[i]);
    free(mArr);
}

void swapPointers(int **a, int **b) {
    int *t = *a;
    *a = *b;
    *b = t;
}

void swapRows(Matrix m, int i1, int i2) {
    assert(0 <= i1 && 0 <= i2);
    assert(i1 < m.nRows && i2 < m.nRows);

    swapPointers(&m.val[i1], &m.val[i2]);
}

void swapInt(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void swapCols(Matrix m, int j1, int j2) {
    assert(0 <= j1 && 0 <= j2);
    assert(j1 < m.nCols && j2 < m.nCols);

    for (int i = 0; i < m.nRows; ++i)
        swapInt(&m.val[i][j1], &m.val[i][j2]);
}

int getMinValIndex(const int *arr, int n) {
    int minIndex = 0;
    for (int i = 1; i < n; ++i)
        if (arr[i] < arr[minIndex])
            minIndex = i;

    return minIndex;
}

int getMaxValIndex(const int *arr, int n) {
    int maxIndex = 0;
    for (int i = 1; i < n; ++i)
        if (arr[i] > arr[maxIndex])
            maxIndex = i;

    return maxIndex;
}

Position getMaxValPosMatrix(Matrix m) {
    int iMin = 0;
    int jMin = getMaxValIndex(m.val[0], m.nCols);
    for (int i = 1; i < m.nRows; ++i) {
        int j = getMaxValIndex(m.val[i], m.nCols);
        if (m.val[i][j] > m.val[iMin][jMin]) {
            iMin = i;
            jMin = j;
        }
    }

    return (Position) {iMin, jMin};
}

Position getMinValPosMatrix(Matrix m) {
    int iMin = 0;
    int jMin = getMinValIndex(m.val[0], m.nCols);
    for (int i = 1; i < m.nRows; ++i) {
        int j = getMinValIndex(m.val[i], m.nCols);
        if (m.val[i][j] < m.val[iMin][jMin]) {
            iMin = i;
            jMin = j;
        }
    }

    return (Position) {iMin, jMin};
}

bool isEMatrix(Matrix m) {
    if (!isSquareMatrix(m))
        return false;

    for (int i = 0; i < m.nRows; ++i)
        for (int j = 0; j < m.nCols; ++j)
            if ((i == j) != m.val[i][j])
                return false;

    return true;
}

bool isSymmetricMatrix(Matrix m) {
    if (!isSquareMatrix(m))
        return false;

    for (int i = 0; i < m.nRows; ++i)
        for (int j = i + 1; j < m.nCols; ++j)
            if (m.val[i][j] != m.val[j][i])
                return false;

    return true;
}

bool isSquareMatrix(Matrix m) {
    return m.nRows == m.nCols;
}

bool areTwoMatricesEqual(Matrix m1, Matrix m2) {
    if (m1.nRows != m2.nRows || m1.nCols != m2.nCols)
        return false;

    for (int i = 0; i < m1.nRows; ++i)
        if (memcmp(m1.val[i], m2.val[i], m1.nCols))
            return false;

    return true;
}

void selectionSortRowsMatrixByRowCriteria(Matrix m, int (*criteria)(const int *, int)) {
    int *criteriaVal = (int *) malloc(sizeof(int) * m.nRows);
    for (int i = 0; i < m.nRows; i++)
        criteriaVal[i] = criteria(m.val[i], m.nCols);

    for (int i = 0; i < m.nRows; i++) {
        int minIndex = i;
        for (int j = i + 1; j < m.nRows; j++)
            if (criteriaVal[j] < criteriaVal[minIndex])
                minIndex = j;
        if (i != minIndex) {
            swapInt(&criteriaVal[i], &criteriaVal[minIndex]);
            swapRows(m, i, minIndex);
        }
    }

    free(criteriaVal);
}

void selectionSortColsMatrixByColCriteria(Matrix m, int (*criteria)(const int *, int)) {
    int *criteriaVal = (int *) malloc(sizeof(int) * m.nCols);
    int *buffer = (int *) malloc(sizeof(int) * m.nRows);
    for (int j = 0; j < m.nCols; j++) {
        for (int i = 0; i < m.nRows; ++i)
            buffer[i] = m.val[i][j];
        criteriaVal[j] = criteria(buffer, m.nCols);
    }
    free(buffer);

    for (int i = 0; i < m.nCols; i++) {
        int minIndex = i;
        for (int j = i + 1; j < m.nCols; j++)
            if (criteriaVal[j] < criteriaVal[minIndex])
                minIndex = j;
        if (i != minIndex) {
            swapInt(&criteriaVal[i], &criteriaVal[minIndex]);
            swapCols(m, i, minIndex);
        }
    }
}

void insertionSortRowsMatrixByRowCriteria(Matrix m, int (*criteria)(const int *, int)) {
    int *criteriaVal = (int *) malloc(sizeof(int) * m.nRows);
    for (int i = 0; i < m.nRows; i++)
        criteriaVal[i] = criteria(m.val[i], m.nCols);

    for (int i = 1; i < m.nRows; ++i) {
        int tempVal = criteriaVal[i];
        int *tempRow = m.val[i];
        int j = i;
        while (j >= 0 && tempVal < criteriaVal[j - 1]) {
            m.val[j] = m.val[j - 1];
            criteriaVal[j] = criteriaVal[j - 1];
            j--;
        }

        m.val[j] = tempRow;
        criteriaVal[j] = tempVal;
    }

    free(criteriaVal);
}

void insertionSortRowsMatrixByRowCriteriaF(Matrix m, float (*criteria)(const int *, int)) {
    float *criteriaVal = (float *) malloc(sizeof(int) * m.nRows);
    for (int i = 0; i < m.nRows; i++)
        criteriaVal[i] = criteria(m.val[i], m.nCols);

    for (int i = 1; i < m.nRows; ++i) {
        float tempVal = criteriaVal[i];
        int *tempRow = m.val[i];
        int j = i;
        while (j >= 0 && tempVal < criteriaVal[j - 1]) {
            m.val[j] = m.val[j - 1];
            criteriaVal[j] = criteriaVal[j - 1];
            j--;
        }

        m.val[j] = tempRow;
        criteriaVal[j] = tempVal;
    }

    free(criteriaVal);
}


void inputMatrix(Matrix *m) {
    for (int i = 0; i < m->nRows; ++i)
        for (int j = 0; j < m->nCols; ++j)
            scanf("%d", &m->val[i][j]);
}

void inputMatrices(Matrix *mArr, int nMatrices) {
    for (int k = 0; k < nMatrices; ++k)
        inputMatrix(&mArr[k]);
}

void outputMatrix(Matrix m) {
    for (int i = 0; i < m.nRows; ++i) {
        printf("| ");
        for (int j = 0; j < m.nCols; ++j)
            printf("%d ", m.val[i][j]);
        printf("|\n");
    }
}

void transposeSquareMatrix(Matrix m) {
    assert(isSquareMatrix(m));

    for (int i = 0; i < m.nRows; ++i)
        for (int j = i + 1; j < m.nCols; ++j)
            swapInt(&m.val[i][j], &m.val[j][i]);
}

void transposeMatrix(Matrix *m) {
    Matrix t = getMemMatrix(m->nCols, m->nRows);
    for (int i = 0; i < m->nRows; i++)
        for (int j = 0; j < m->nCols; j++)
            t.val[j][i] = m->val[i][j];

    freeMemMatrix(*m);
    *m = t;
}

Matrix *createArrayOfMatrixFromArray(const int *values, int nMatrices, int nRows, int nCols) {
    Matrix *ms = getMemArrayOfMatrices(nMatrices, nRows, nCols);
    int iArr = 0;
    for (int k = 0; k < nMatrices; k++)
        for (int i = 0; i < nRows; i++)
            for (int j = 0; j < nCols; j++)
                ms[k].val[i][j] = values[iArr++];

    return ms;
}

Matrix createMatrixFromArray(const int *a, int nRows, int nCols) {
    Matrix m = getMemMatrix(nRows, nCols);
    int k = 0;
    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            m.val[i][j] = a[k++];

    return m;
}

void outputMatrices(Matrix *mArr, int nMatrices) {
    for (int k = 0; k < nMatrices; ++k)
        outputMatrix(mArr[k]);
}

int maxInt(int a, int b) {
    return a > b ? a : b;
}

long long getSumOfMaxesOfPseudoDiagonal(Matrix m) {
    long long sum = 0;
    for (int iter = 1; iter < m.nRows; ++iter) {
        int i = iter;
        int j = 0;
        int maxVal = m.val[i][j];
        while (++i < m.nRows && ++j < m.nCols) {
            maxVal = maxInt(maxVal, m.val[i][j]);
        }

        sum += maxVal;
    }

    for (int iter = 1; iter < m.nCols; ++iter) {
        int i = 0;
        int j = iter;
        int maxVal = m.val[i][j];
        while (++i < m.nRows && ++j < m.nCols) {
            maxVal = maxInt(maxVal, m.val[i][j]);
        }

        sum += maxVal;
    }

    return sum;
}



float getDistance(const int *a, int n) {
    double sum = 0;
    for (int i = 0; i < n; ++i)
        sum += (double) a[i] * a[i];

    return (float) sqrt(sum);
}

void sortByDistances(Matrix m) {
    insertionSortRowsMatrixByRowCriteriaF(m, getDistance);
}

bool isNonDescendingSorted(const int *a, int n) {
    int prevElem = a[0];
    for (int i = 1; i < n; ++i) {
        if (a[i] < prevElem)
            return false;
        prevElem = a[i];
    }

    return true;
}

bool hasAllNonDescendingRows(Matrix m) {
    for (int i = 0; i < m.nRows; ++i)
        if (!isNonDescendingSorted(m.val[i], m.nCols))
            return false;

    return true;
}

int countNonDescendingRowsMatrices(Matrix *ms, int nMatrix) {
    int count = 0;
    for (int i = 0; i < nMatrix; ++i)
        count += hasAllNonDescendingRows(ms[i]);

    return count;
}

long long getNSpecialElement(Matrix m) {
    long long count = 0;
    for (int i = 0; i < m.nRows; ++i)
        for (int j = 0; j < m.nCols; ++j) {
            bool isSpecialElem = true;
            for (int j1 = j - 1; j1 >= 0; --j1)
                if (m.val[i][j] <= m.val[i][j1]) {
                    isSpecialElem = false;
                    break;
                }

            for (int j2 = j + 1; j2 < m.nCols; ++j2)
                if (m.val[i][j] >= m.val[i][j2]) {
                    isSpecialElem = false;
                    break;
                }

            if (isSpecialElem) count++;
        }

    return count;
}

long long getScalarProductRowAndCol(Matrix m, int i, int j) {
    assert(isSquareMatrix(m));

    long long sum = 0;
    for (int iter = 0; iter < m.nRows; ++iter)
        sum += (long long) m.val[i][iter] * m.val[iter][j];

    return sum;
}

long long getSpecialScalarProduct(Matrix m) {
    int indexRowWithMax = getMaxValPosMatrix(m).iRow;
    int indexColWithMin = getMinValPosMatrix(m).iCol;

    return getScalarProductRowAndCol(
            m,
            indexRowWithMax,
            indexColWithMin
    );
}