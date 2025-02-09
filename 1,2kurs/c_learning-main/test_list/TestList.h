//
// Created by dmitr on 08.03.2023.
//

#ifndef MAIN_C_TESTLIST_H
#define MAIN_C_TESTLIST_H

#endif //MAIN_C_TESTLIST_H

#define ASSERT_STRING(got, expected) ASSERTString_(got, expected, \
__FILE__, __FUNCTION__, __LINE__)

#define ASSERT(res) ASSERT_(res, \
__FILE__, __FUNCTION__, __LINE__)

void ASSERT_(int res, const char *fileName, const char *funcName, int line);

void ASSERTString_(char *got, char *expected, const char *fileName, const char *funcName, int line);

void test_Matrix_functional();

void test_strlen();

void test_find_functions();

void test_strcmp();

void test_copy();

void test_copyIf();

void test_copyIfReverse();

void timeExperiment();

void amountComparesExperiment();

void test_Vector();