//
// Created by dmitr on 08.03.2023.
//

#include <time.h>
#include "../libs/OperationOverMatrix.h"
#include "../libs/String_DAA_lib.h"
#include "../libs/Sorts.h"
#include "../libs/Vector.h"

#define ASSERT_STRING(got, expected) ASSERTString_(got, expected, \
__FILE__, __FUNCTION__, __LINE__)

#define ASSERT(res) ASSERT_(res, \
__FILE__, __FUNCTION__, __LINE__)

#define TIME_TEST(testCode, time) { \
clock_t start_time = clock(); \
testCode \
clock_t end_time = clock();\
clock_t sort_time = end_time - start_time; \
time = (double) sort_time / CLOCKS_PER_SEC; \
}

#define AMOUNT_TEST(testCode, counter) { counter = testCode; }

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

typedef struct SortFuncTag {
    void (*sort)(int *a, size_t n);

    char name[64];
} SortFuncTag;

typedef struct SortWithInjectionFuncTag {
    unsigned long long (*sort)(int *a, size_t n);

    char name[64];
} SortWithInjectionFuncTag;

typedef struct GeneratingFuncTag {
    void (*generate)(int *a, size_t n);

    char name[64];
} GeneratingFuncTag;

void generateRandomArray(int *a, unsigned long long n) {
    srand(time(NULL));

    for (int i = 0; i < n; ++i)
        a[i] = rand() * ((i % 5) ? 1 : -1);
}

void generateOrderedArray(int *a, unsigned long long n) {
    for (int i = 0; i < n; ++i)
        a[i] = i;
}

void generateOrderedBackwards(int *a, unsigned long long n) {
    for (int i = 0; i < n; ++i)
        a[i] = (int) n - i - 1;
}

void checkTime(void(*sortFunc)(int *, size_t),
               void (*generateFunc)(int *, size_t),
               size_t size, char *experimentName) {
    static size_t runCounter = 1;
    static int innerBuffer[120000];
    generateFunc(innerBuffer, size);

    printf("Run #%zu| ", runCounter++);
    printf("Name: %s\n", experimentName);

    double time;
    TIME_TEST({ sortFunc(innerBuffer, size); }, time)
    printf("Status: ");
    if (isOrdered(innerBuffer, size)) {
        printf("OK! Time: %.3f s.\n", time);

        char filename[256];
        sprintf(filename, "../data0/%s.csv", experimentName);

        FILE *f = fopen(filename, "a");
        if (f == NULL) {
            printf("FileOpenError %s", filename);
            exit(1);
        }

        fprintf(f, "%zu; %.3f\n", size, time);

        fclose(f);
    } else {
        printf("Wrong!\n");
        outputArray(innerBuffer, size);

        exit(1);
    }
}

void timeExperiment() {
// описание функций сортировки
    SortFuncTag sorts[] = {
            {bubbleSort,    "bubbleSort"},
            {selectionSort, "selectionSort"},
            {insertionSort, "insertionSort"},
            {combSort,      "combSort"},
            {ShellsSort,    "ShellsSort"},
            {digitalSort,   "digitalSort"}
    };
    const unsigned FUNCS_N = ARRAY_SIZE(sorts);

// описание функций генерации
    GeneratingFuncTag GeneratingFuncTags[] = {
// генерируется случайный массив
            {generateRandomArray,      "random"},
// генерируется массив 0, 1, 2, ..., n - 1
            {generateOrderedArray,     "ordered"},
// генерируется массив n - 1, n - 2, ..., 0
            {generateOrderedBackwards, "orderedBackwards"}
    };
    const unsigned CASES_N = ARRAY_SIZE(GeneratingFuncTags);

// запись статистики в файл
    for (size_t size = 60000; size <= 120000; size += 10000) {
        printf("------------------------------\n");
        printf("Size: %zu\n", size);
        for (int i = 0; i < FUNCS_N; i++) {
            for (int j = 0; j < CASES_N; j++) {
// генерация имени файла
                static char filename[128];
                sprintf(filename, "%s_%s_time",
                        sorts[i].name, GeneratingFuncTags[j].name);
                checkTime(sorts[i].sort,
                          GeneratingFuncTags[j].generate,
                          size, filename);
            }
        }
        printf("\n");
    }
}

void checkAmountCompares(unsigned long long (*sortFunc)(int *, size_t),
                         void (*generateFunc)(int *, size_t),
                         size_t size, char *experimentName) {
    static size_t runCounter = 1;
    static int innerBuffer[110000];
    generateFunc(innerBuffer, size);

    printf("Run #%zu| ", runCounter++);
    printf("Name: %s\n", experimentName);

    unsigned long long countOperations;
    AMOUNT_TEST(sortFunc(innerBuffer, size), countOperations)

    printf("Status: ");
    if (isOrdered(innerBuffer, size)) {
        printf("OK! Amount of compares: %llu\n", countOperations);
// запись в файл
        char filename[256];
        sprintf(filename, "../data1/%s.csv", experimentName);

        FILE *f = fopen(filename, "a");
        if (f == NULL) {
            printf("FileOpenError %s", filename);
            exit(1);
        }

        fprintf(f, "%zu; %llu\n", size, countOperations);

        fclose(f);
    } else {
        printf("Wrong!\n");
        outputArray(innerBuffer, size);

        exit(1);
    }
}

void amountComparesExperiment() {
    SortWithInjectionFuncTag sortsWithInjection[] = {
            {bubbleSortInjected,    "bubbleSort"},
            {selectionSortInjected, "selectionSort"},
            {insertionSortInjected, "insertionSort"},
            {combSortInjected,      "combSort"},
            {ShellsSortInjected,    "ShellsSort"},
            {digitalSortInjected,   "digitalSort"}
    };
    const unsigned FUNCS_N = ARRAY_SIZE(sortsWithInjection);

    GeneratingFuncTag GeneratingFuncTags[] = {
// генерируется случайный массив
            {generateRandomArray,      "random"},
// генерируется массив 0, 1, 2, ..., n - 1
            {generateOrderedArray,     "ordered"},
// генерируется массив n - 1, n - 2, ..., 0
            {generateOrderedBackwards, "orderedBackwards"}
    };
    const unsigned CASES_N = ARRAY_SIZE(GeneratingFuncTags);

    for (size_t size = 600; size <= 1200; size += 100) {
        printf("------------------------------\n");
        printf("Size: %zu\n", size);
        for (int i = 0; i < FUNCS_N; i++) {
            for (int j = 0; j < CASES_N; j++) {
// генерация имени файла
                static char filename[128];
                sprintf(filename, "%s_%s_compares",
                        sortsWithInjection[i].name, GeneratingFuncTags[j].name);
                checkAmountCompares(
                        sortsWithInjection[i].sort,
                        GeneratingFuncTags[j].generate,
                        size,
                        filename
                );
            }
        }
        printf("\n");
    }
}

void ASSERTString_(char *got, char *expected, const char *fileName, const char *funcName, int line) {
    if (strcmp_(expected, got)) {
        fprintf(stderr, "File %s\n", fileName);
        fprintf(stderr, "%s - failed on line %d\n", funcName, line);
        fprintf(stderr, "Got: \"%s\"\n", got);
        fprintf(stderr, "Expected: \"%s\"\n\n", expected);
    } else
        fprintf(stderr, "%s - OK\n", funcName);
}

void ASSERT_(int res, const char *fileName, const char *funcName, int line) {
    if (!res) {
        fprintf(stderr, "File %s\n", fileName);
        fprintf(stderr, "%s - failed on line %d\n", funcName, line);
    } else
        fprintf(stderr, "%s - OK\n", funcName);
}


void test_1() {
    Matrix m1 = createMatrixFromArray((int[]) {
            1, 2,
            5, 6,
            3, 4}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            1, 2,
            5, 6,
            3, 4}, 3, 2);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_2() {
    Matrix m1 = createMatrixFromArray((int[]) {
            1, 2,
            5, 6,
            3, 4}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            5, 6,
            1, 2,
            3, 4}, 3, 2);

    swapRows(m1, 0, 1);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_3() {
    Matrix m1 = createMatrixFromArray((int[]) {
            1, 2,
            5, 6,
            3, 4}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            2, 1,
            6, 5,
            4, 3}, 3, 2);

    swapCols(m1, 0, 1);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_getSumOfMaxesOfPseudoDiagonal() {
    Matrix m1 = createMatrixFromArray((int[]) {
            3, 2, 5, 4,
            1, 3, 6, 3,
            3, 2, 1, 2}, 3, 4);
    Matrix m2 = createMatrixFromArray((int[]) {
            1,
            4,
            5,
            8}, 4, 1
    );

    Matrix m3 = createMatrixFromArray((int[]) {
            3}, 1, 1);

    ASSERT(getSumOfMaxesOfPseudoDiagonal(m1) == 20);
    ASSERT(getSumOfMaxesOfPseudoDiagonal(m2) == 17);
    ASSERT(getSumOfMaxesOfPseudoDiagonal(m3) == 0);

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_sortByDistances() {
    Matrix m1 = createMatrixFromArray((int[]) {
            3, 2, 5, 4,
            1, 3, 6, 3,
            3, 2, 1, 2}, 3, 4);
    Matrix m2 = createMatrixFromArray((int[]) {
            3, 2, 1, 2,
            3, 2, 5, 4,
            1, 3, 6, 3}, 3, 4);

    sortByDistances(m1);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);

}

void test_countNonDescendingRowsMatrices() {
    Matrix *ms1 = createArrayOfMatrixFromArray((int[]) {
            7, 1, 1, 1,
            1, 6, 2, 2,
            5, 4, 2, 3,
            1, 3, 7, 9}, 4, 2, 2
    );

    Matrix *ms2 = createArrayOfMatrixFromArray((int[]) {
            3, 5,
            4, 7,
            7, 7}, 3, 1, 2
    );


    ASSERT(countNonDescendingRowsMatrices(ms1, 4) == 2);
    ASSERT(countNonDescendingRowsMatrices(ms2, 3) == 3);

    freeMemMatrices(ms1, 4);
    freeMemMatrices(ms2, 3);
}

void test_getNSpecialElement() {
    Matrix m1 = createMatrixFromArray((int[]) {
            2, 3, 5, 5, 4,
            6, 2, 3, 8, 12,
            12, 12, 2, 1, 2
    }, 3, 5);

    Matrix m2 = createMatrixFromArray((int[]) {
            2, 3, 3,
            3, 3, 3,
            3, 3, 4}, 3, 3
    );

    ASSERT(getNSpecialElement(m1) == 4);
    ASSERT(getNSpecialElement(m2) == 2);

    freeMemMatrix(m1);
    freeMemMatrix(m2);

}

void test_getSpecialScalarProduct() {
    Matrix m1 = createMatrixFromArray((int[]) {
            2, 3, 4,
            5, 6, 9,
            10, 8, 7}, 3, 3
    );

    ASSERT(getSpecialScalarProduct(m1) == 130);

    freeMemMatrix(m1);
}

int getMin(const int *arr, int n) {
    int minVal = arr[0];
    for (int i = 1; i < n; ++i)
        if (arr[i] < minVal)
            minVal = arr[i];

    return minVal;
}

void test_4() {
    Matrix m1 = createMatrixFromArray((int[]) {
            1, 2,
            5, 6,
            3, 4}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            1, 2,
            3, 4,
            5, 6}, 3, 2);

    insertionSortRowsMatrixByRowCriteria(m1, getMin);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_5() {
    Matrix m1 = createMatrixFromArray((int[]) {
            1, 2,
            5, 6,
            3, 4}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            1, 2,
            3, 4,
            5, 6}, 3, 2);

    selectionSortRowsMatrixByRowCriteria(m1, getMin);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_6() {
    Matrix m1 = createMatrixFromArray((int[]) {
            2, 1,
            6, 5,
            4, 3}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            1, 2,
            5, 6,
            3, 4}, 3, 2);

    selectionSortColsMatrixByColCriteria(m1, getMin);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_7() {
    Matrix m1 = createMatrixFromArray((int[]) {
            2, 1,
            6, 5,
            4, 3}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            4, 5, 6,
            1, 2, 3,
            7, 8, 9}, 3, 3);

    ASSERT(isSquareMatrix(m1) == 0);
    ASSERT(isSquareMatrix(m2) == 1);

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_8() {
    Matrix m1 = createMatrixFromArray((int[]) {
            2, 1,
            'h', 5,
            4, 3}, 3, 2);
    Matrix m2 = createMatrixFromArray((int[]) {
            4, 5, 6,
            1, 'e', 3,
            7, 8, 9}, 3, 3);
    Matrix mSym = createMatrixFromArray((int[]) {
            'l', 3, 0,
            3, 1, 2,
            0, 2, 'p'}, 3, 3);

    ASSERT(isSymmetricMatrix(m1) == 0);
    ASSERT(isSymmetricMatrix(m2) == 0);
    ASSERT(isSymmetricMatrix(mSym) == 1 && '!');

    freeMemMatrix(m1);
    freeMemMatrix(m2);
    freeMemMatrix(mSym);
}


void test_9() {
    Matrix m1 = createMatrixFromArray((int[]) {
            2, 1,
            6, 5,
            4, 3}, 3, 2);

    Position minPos = getMinValPosMatrix(m1);
    ASSERT(minPos.iRow == 0 && minPos.iCol == 1);

    freeMemMatrix(m1);
}

void test_10() {
    Matrix m1 = createMatrixFromArray((int[]) {
            2, 1,
            6, 5,
            4, 3}, 3, 2);

    Position maxPos = getMaxValPosMatrix(m1);
    ASSERT(maxPos.iRow == 1 && maxPos.iCol == 0);

    freeMemMatrix(m1);
}

void test_11() {
    Matrix E = createMatrixFromArray((int[]) {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1}, 3, 3);
    Matrix E_broken = createMatrixFromArray((int[]) {
            1, 2,
            3, 9,
            5, 8}, 3, 2);
    Matrix E_broken_too = createMatrixFromArray((int[]) {
            1, 2, 3,
            3, 9, 3,
            5, 8, 7}, 3, 3);

    ASSERT(isEMatrix(E) == 1);
    ASSERT(isEMatrix(E_broken) == 0);
    ASSERT(isEMatrix(E_broken_too) == 0);

    freeMemMatrix(E);
    freeMemMatrix(E_broken);
    freeMemMatrix(E_broken_too);
}

void test_12() {
    Matrix m1 = createMatrixFromArray((int[]) {
            4, 1, 7,
            5, 2, 8,
            6, 3, 9}, 3, 3);
    Matrix m2 = createMatrixFromArray((int[]) {
            4, 5, 6,
            1, 2, 3,
            7, 8, 9}, 3, 3);

    transposeSquareMatrix(m1);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_13() {
    Matrix m1 = createMatrixFromArray((int[]) {
            4, 1, 7,
            5, 2, 8}, 2, 3);
    Matrix m2 = createMatrixFromArray((int[]) {
            4, 5,
            1, 2,
            7, 8}, 3, 2);

    transposeMatrix(&m1);

    ASSERT(areTwoMatricesEqual(m1, m2));

    freeMemMatrix(m1);
    freeMemMatrix(m2);
}

void test_Matrix_functional() {
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
    test_10();
    test_11();
    test_12();
    test_13();
}

void test_strlen() {
    char *s1 = "Hi";
    char s2[10] = "\tHello\t";

    ASSERT(strlen1(s1) == 2);
    ASSERT(strlen1(s2) == 7);
    ASSERT(strlen2(s1) == 2);
    ASSERT(strlen2(s2) == 7);
    ASSERT(strlen_(s1) == 2);
    ASSERT(strlen_(s2) == 7);
}

void test_find_functions() {
    char *s1 = "Hel\tlo";
    char *s2 = "Hello";
    char *s3 = "\n  0";
    char *s4 = "\t \n\t";

    ASSERT(find(s1, s1 + strlen_(s1), 'o') == &s1[5]);
    ASSERT(find(s1, s1 + strlen_(s1), 'N') == &s1[6]);

    ASSERT(findNonSpace(s1) == s1);
    ASSERT(findNonSpace(s2) == s2);
    ASSERT(findNonSpace(s3) == &s3[3]);
    ASSERT(findNonSpace(s4) == &s4[4]);

    ASSERT(findSpace(s1) == &s1[3]);
    ASSERT(findSpace(s2) == &s2[5]);
    ASSERT(findSpace(s3) == s3);

    ASSERT(findNonSpaceReverse(s1 + strlen_(s1) - 1, s1 - 1) == &s1[5]);
    ASSERT(findNonSpaceReverse(s2 + strlen_(s2) - 1, s2 - 1) == &s2[4]);
    ASSERT(findNonSpaceReverse(s3 + strlen_(s3) - 1, s3 - 1) == &s3[3]);
    ASSERT(findNonSpaceReverse(s4 + strlen_(s4) - 1, s4 - 1) == s4 - 1);

    ASSERT(findSpaceReverse(s1 + strlen_(s1) - 1, s1 - 1) == &s1[3]);
    ASSERT(findSpaceReverse(s2 + strlen_(s2) - 1, s2 - 1) == s2 - 1);
    ASSERT(findSpaceReverse(s4 + strlen_(s4) - 1, s4 - 1) == &s4[3]);
}

void test_strcmp() {
    char *s1 = "hello";
    char *s2 = "hello";
    char *s3 = "helllo";
    char *s4 = "Hello";
    char *s5 = "hhhh";
    char *s6 = "hhhhhh";

    ASSERT(strcmp_(s1, s2) == 0);
    ASSERT(strcmp_(s1, s3) > 0);
    ASSERT(strcmp_(s1, s4) > 0);
    ASSERT(strcmp_(s5, s6) < 0);
}

void test_copy() {
    char *s1 = "hello";
    char *s2 = "hell";
    char *s3 = "hello";
    char dist[10] = {};

    copy(s1, s1 + 4, dist);

    ASSERT(strcmp_(dist, s2) == 0);

    copy(s1, s1 + 5, dist);

    ASSERT(strcmp_(dist, s3) == 0);
}

int ignoreUpperCase(int s) {
    return s < 65 || s >= 91;
}

void test_copyIf() {
    char *s1 = "heLlohELLo";
    char *s2 = "heloho";
    char dist[10] = {};

    copyIf(s1, s1 + strlen_(s1),
           dist, ignoreUpperCase);

    ASSERT(strcmp_(dist, s2) == 0);
}

void test_copyIfReverse() {
    char *s1 = "heLlohELLo";
    char *s2 = "oholeh";
    char *s3 = "holeh";
    char dist[10] = {};

    copyIfReverse(s1 + strlen_(s1) - 2, s1 - 1,
                  dist, ignoreUpperCase);

    ASSERT(strcmp_(dist, s3) == 0);

    copyIfReverse(s1 + strlen_(s1) - 1, s1 - 1,
                  dist, ignoreUpperCase);

    ASSERT(strcmp_(dist, s2) == 0);
}

void test_removeExtraSpaces() {
    char s0[] = "";
    char s1[] = " test";
    char s2[] = "test ";
    char s3[] = "  test";
    char s4[] = "test  ";
    char s5[] = "te st";
    char s6[] = "te  st";

    removeExtraSpaces(s0);
    removeExtraSpaces(s1);
    removeExtraSpaces(s2);
    removeExtraSpaces(s3);
    removeExtraSpaces(s4);
    removeExtraSpaces(s5);
    removeExtraSpaces(s6);

    ASSERT_STRING(s0, "");
    ASSERT_STRING(s1, "test");
    ASSERT_STRING(s2, "test");
    ASSERT_STRING(s3, "test");
    ASSERT_STRING(s4, "test");
    ASSERT_STRING(s5, "te st");
    ASSERT_STRING(s6, "te st");
}

void test_getWord() {
    char s1[] = "test1 tt2";

    WordDescriptor w1;
    getWord(s1 + 5, &w1);
    *w1.end = '\0';

    WordDescriptor w2;
    getWord(s1 + 3, &w2);
    *w2.end = '\0';

    WordDescriptor w3;
    getWord(s1, &w3);
    *w3.end = '\0';

    ASSERT_STRING(w1.begin, "tt2");
    ASSERT_STRING(w2.begin, "t1");
    ASSERT_STRING(w3.begin, "test1");
}

void test_digitToEnd() {
    char s1[] = "test1";
    char s2[] = "1test";
    char s3[] = "t1e2s3t";

    WordDescriptor w1;
    WordDescriptor w2;
    WordDescriptor w3;

    getWord(s1, &w1);
    getWord(s2, &w2);
    getWord(s3, &w3);

    digitToEnd(w1);
    digitToEnd(w2);
    digitToEnd(w3);

    ASSERT_STRING(w1.begin, "test1");
    ASSERT_STRING(w2.begin, "test1");
    ASSERT_STRING(w3.begin, "test123");
}

void test_replaceDigitsBySpaces() {
    char s0[] = "";
    char s1[10] = "test2";
    char s2[10] = "2test";
    char s3[15] = "t1e2s3t";

    replaceDigitsBySpaces(s0);
    replaceDigitsBySpaces(s1);
    replaceDigitsBySpaces(s2);
    replaceDigitsBySpaces(s3);

    ASSERT_STRING(s0, "");
    ASSERT_STRING(s1, "test  ");
    ASSERT_STRING(s2, "  test");
    ASSERT_STRING(s3, "t e  s   t");
}

void test_replace() {
    char s01[] = "";
    char s02[] = "";
    char s1[20] = "testtesttest";
    char s2[20] = "testtesttest";
    char s3[20] = "testtesttest";
    char s4[20] = "testtesttest";
    char s5[20] = "tttstt";
    char s6[20] = "test test test";


    replace(s01, "test", "1");
    replace(s02, "1", "test");
    replace(s1, "est", "1");
    replace(s2, "t", "bed");
    replace(s3, "", "breakHAHA");
    replace(s4, "te", "");
    replace(s5, "tt", "t");
    replace(s6, "t ", "buga");


    ASSERT_STRING(s01, "");
    ASSERT_STRING(s02, "");
    ASSERT_STRING(s1, "t1t1t1");
    ASSERT_STRING(s2, "bedesbedbedesbedbedesbed");
    ASSERT_STRING(s3, "testtesttest");
    ASSERT_STRING(s4, "ststst");
    ASSERT_STRING(s5, "ttst");
    ASSERT_STRING(s6, "tesbugatesbugatest");
}

void test_isLexicographicalOrder() {
    char s1[] = "";
    char s2[] = "1";
    char s3[] = "1 2 3 4";
    char s4[] = "1 2 4 3";
    char s5[] = "12 33 34 44";

    ASSERT(isLexicographicalOrder(s1));
    ASSERT(isLexicographicalOrder(s2));
    ASSERT(isLexicographicalOrder(s3));
    ASSERT(isLexicographicalOrder(s4));
    ASSERT(isLexicographicalOrder(s5));
}

void test_getBagOfWords() {
    char s1[] = "";
    char s2[] = "test";
    char s3[] = "test   ";
    char s4[] = "   test";
    char s5[] = "tt1 teeeeest2 test3   test444";

    BagOfWords b1;
    BagOfWords b2;
    BagOfWords b3;
    BagOfWords b4;
    BagOfWords b5;

    getBagOfWords(&b1, s1);
    getBagOfWords(&b2, s2);
    getBagOfWords(&b3, s3);
    getBagOfWords(&b4, s4);

    getBagOfWords(&b5, s5);
    *b5.words[0].end = '\0';
    *b5.words[1].end = '\0';
    *b5.words[2].end = '\0';

    ASSERT_STRING(b1.words[0].begin, "");

    ASSERT_STRING(b2.words[0].begin, "test");
    ASSERT_STRING(b3.words[0].begin, "test   ");
    ASSERT_STRING(b4.words[0].begin, "test");

    ASSERT_STRING(b5.words[0].begin, "tt1");
    ASSERT_STRING(b5.words[1].begin, "teeeeest2");
    ASSERT_STRING(b5.words[2].begin, "test3");
    ASSERT_STRING(b5.words[3].begin, "test444");
}

void test_getWordAlternation() {
    char s1[] = "test1 test2";
    char s2[] = "test3 test4";
    char dist12[41];
    char s3[] = "test1";
    char s4[] = "test2 test3";
    char dist34[41];
    char s5[] = "test1 test2";
    char s6[] = "test3";
    char dist56[41];
    char s7[] = "test1";
    char s8[] = "";
    char dist78[41];
    char s9[] = "";
    char s0[] = "test1";
    char dist90[41];

    getWordAlternation(s1, s2, dist12);
    getWordAlternation(s3, s4, dist34);
    getWordAlternation(s5, s6, dist56);
    getWordAlternation(s7, s8, dist78);
    getWordAlternation(s9, s0, dist90);

    ASSERT_STRING(dist12, "test1 test3 test2 test4");
    ASSERT_STRING(dist34, "test1 test2 test3");
    ASSERT_STRING(dist56, "test1 test3 test2");
    ASSERT_STRING(dist78, "test1");
    ASSERT_STRING(dist90, "test1");
}

void test_pushBack_emptyVector() {
    Vector v = createVector(5);
    pushBack(&v, 1);
    pushBack(&v, 2);
    assert(v.capacity == 5);
    assert(v.size == 2);

    assert(getVectorValue(v, 0) == 1);
    assert(getVectorValue(v, 1) == 2);

    deleteVector(&v);
}

void test_pushBack_fullVector() {
    Vector v = createVector(2);
    pushBack(&v, 1);
    pushBack(&v, 2);
    pushBack(&v, 3);
    pushBack(&v, 4);
    assert(v.capacity == 4);
    assert(v.size == 4);

    assert(getVectorValue(v, 2) == 3);
    assert(getVectorValue(v, 3) == 4);

    deleteVector(&v);
}

void test_popBack_notEmptyVector() {
    Vector v = createVector(0);
    pushBack(&v, 10);
    assert(v.size == 1);
    popBack(&v);
    assert(v.size == 0);
    assert(v.capacity == 1);

    deleteVector(&v);
}

void test_atVector_notEmptyVector() {
    Vector v = createVector(10);
    v.data = (int[]) {1, 2, 3, 4};
    v.size = 4;

    assert(atVector(v, 1) == &v.data[1]);
    assert(atVector(v, 0) == &v.data[0]);
}

void test_atVector_emptyVector() {
    Vector v2 = createVector(10);

    assert(atVector(v2, 0) == v2.data);
}

void test_atVector_requestToLastElement() {
    Vector v = createVector(10);
    v.data = (int[]) {1, 2, 3, 4};
    v.size = 4;

    assert(atVector(v, 3) == &v.data[3]);
}

void test_back_oneElementInVector() {
    Vector v = createVector(10);
    pushBack(&v, 1);

    assert(back(v) == v.data);
}

void test_back_zeroElementInVector() {
    Vector v = createVector(10);

    assert(back(v) == v.data);
}

void test_front_oneElementInVector() {
    Vector v = createVector(10);
    pushBack(&v, 1);

    assert(front(v) == v.data);
}

void test_front_zeroElementInVector() {
    Vector v = createVector(10);

    assert(front(v) == v.data);
}

void test_arrayToVector_notEmptyArray() {
    Vector v = createVector(0);
    int a[] = {1, 2, 3, 4};
    arrayToVector(&v, a, 4);

    for (int i = 0; i < 4; ++i) {
        assert(v.data[i] == a[i]);
    }
}

void test_Vector() {
    test_pushBack_emptyVector();
    test_pushBack_fullVector();
    test_popBack_notEmptyVector();
    test_atVector_notEmptyVector();
    test_atVector_emptyVector();
    test_atVector_requestToLastElement();
    test_back_oneElementInVector();
    test_back_zeroElementInVector();
    test_front_oneElementInVector();
    test_front_zeroElementInVector();
    test_arrayToVector_notEmptyArray();
}