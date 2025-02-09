//#include <stdio.h>
//
//void outputLogicTable3(int (*f)(int, int, int)) {
//    printf("| A B C | F |\n");
//    for (int A = 0; A < 2; ++A)
//        for (int B = 0; B < 2; ++B)
//            for (int C = 0; C < 2; ++C)
//                printf("| %d %d %d | %d |\n", A, B, C, f(A, B, C));
//}
//
//int isLogicTableEquals(int (*f1)(int, int, int),
//                       int (*f2)(int, int, int)) {
//    for (int A = 0; A < 2; ++A)
//        for (int B = 0; B < 2; ++B)
//            for (int C = 0; C < 2; ++C)
//                if (f1(A, B, C) != f2(A, B, C)) return 0;
//
//    return 1;
//}
//
//int func1(int A, int B, int C) {
//    return A && (!(B || C)) || B && (!(A || C)) || C && (!(A || B));
//}
//
//int func2(int A, int B, int C) {
//    return ((((A && !B) && !((A && !B) && !(A && !C))) && !((B && !A) && !((B && !A) && !(B && !C)))) !=
//            ((B && !A) && !((B && !A) && !(B && !C))) && !((C && !A) && !((C && !A) && !(C && !B)))) !=
//           ((C && !A) && !((C && !A) && !(C && !B)));
//}
//
//
////int main() {
////    outputLogicTable3(func1);
////    printf("\n");
////    outputLogicTable3(func2);
////
////    printf("result is %s", isLogicTableEquals(func1, func2) ? "equal\n" : "not equal\n");
////
////    return 0;
////}
//
//#include <stdio.h>
//#include "libs/OperationOverSet.h"
//
//void outputSet(int *A, int nA, int *B, int nB, int *C, int nC,
//               void (*f)(int *, int, int *, int, int *, int, int *, int *)) {
//    int res1[nA + nB + nC];
//    int nRes1;
//    f(A, nA, B, nB, C, nC, res1, &nRes1);
//
//    printf("{ ");
//    for (int i = 0; i < nRes1; ++i)
//        printf("%d ", res1[i]);
//    printf("}\n");
//}
//
//int isSetsEquals(int *A, int nA, int *B, int nB, int *C, int nC,
//                 void (*f1)(int *, int, int *, int, int *, int, int *, int *),
//                 void (*f2)(int *, int, int *, int, int *, int, int *, int *)) {
//    int res1[nA + nB + nC];
//    int nRes1;
//    f1(A, nA, B, nB, C, nC, res1, &nRes1);
//
//    int res2[nA + nB + nC];
//    int nRes2;
//    f2(A, nA, B, nB, C, nC, res2, &nRes2);
//
//    if (nRes1 != nRes2) return 0;
//
//    for (int i = 0; i < nRes1; ++i) {
//        int flag = 1;
//        for (int j = 0; j < nRes2; ++j)
//            if (res1[i] == res2[j]) {
//                flag = 0;
//                break;
//            }
//
//        if (flag)
//            return 0;
//    }
//
//    return 1;
//}
//
//void func1_(int *A, int nA, int *B, int nB, int *C, int nC, int *res,
//            int *nRes) {
//    int bufAB[nA + nB + nC];
//    int nBufAB;
//    differenceForSortSet(A, nA, B, nB, bufAB, &nBufAB);
//
//    int bufAC[nA + nB + nC];
//    int nBufAC;
//    differenceForSortSet(A, nA, C, nC, bufAC, &nBufAC);
//
//    int bufBA[nA + nB + nC];
//    int nBufBA;
//    differenceForSortSet(B, nB, A, nA, bufBA, &nBufBA);
//
//    int bufBC[nA + nB + nC];
//    int nBufBC;
//    differenceForSortSet(B, nB, C, nC, bufBC, &nBufBC);
//
//    int bufCA[nA + nB + nC];
//    int nBufCA;
//    differenceForSortSet(C, nC, A, nA, bufCA, &nBufCA);
//
//    int bufCB[nA + nB + nC];
//    int nBufCB;
//    differenceForSortSet(C, nC, B, nB, bufCB, &nBufCB);
//
//    int bufABAC[nBufAB + nBufAC];
//    int nBufABAC;
//    intersectionForSortSet(bufAB, nBufAB, bufAC, nBufAC, bufABAC, &nBufABAC);
//
//    int bufBABC[nBufBA + nBufBC];
//    int nBufBABC;
//    intersectionForSortSet(bufBA, nBufBA, bufBC, nBufBC, bufBABC, &nBufBABC);
//
//    int bufCACB[nBufCA + nBufAC];
//    int nBufCACB;
//    intersectionForSortSet(bufCA, nBufCA, bufCB, nBufCB, bufCACB, &nBufCACB);
//
//    int bufABACBABC[nBufABAC + nBufBABC];
//    int nBufABACBABC;
//    differenceForSortSet(bufABAC, nBufABAC, bufBABC, nBufBABC, bufABACBABC, &nBufABACBABC);
//
//    int bufABACBABCBABC[nBufABAC + nBufBABC];
//    int nBufABACBABCBABC;
//    symmetricalDifferenceForSortSet(bufABAC, nBufABAC, bufBABC, nBufBABC, bufABACBABCBABC, &nBufABACBABCBABC);
//
//    int bufABACBABCBABCCACB[nBufABACBABCBABC + nBufCACB];
//    int nBufABACBABCBABCCACB;
//    differenceForSortSet(bufABACBABCBABC, nBufABACBABCBABC, bufCACB, nBufCACB, bufABACBABCBABCCACB,
//                         &nBufABACBABCBABCCACB);
//
//    symmetricalDifferenceForSortSet(bufABACBABCBABCCACB, nBufABACBABCBABCCACB, bufCACB, nBufCACB, res, nRes);
//}
//
//void func2_(int *A, int nA, int *B, int nB, int *C, int nC, int *res,
//            int *nRes) {
//    int bufAB[nA + nB + nC];
//    int nBufAB;
//    differenceForSortSet(A, nA, B, nB, bufAB, &nBufAB);
//
//    int bufAC[nA + nB + nC];
//    int nBufAC;
//    differenceForSortSet(A, nA, C, nC, bufAC, &nBufAC);
//
//    int bufABAC[nBufAB + nBufAC];
//    int nBufABAC;
//    differenceForSortSet(bufAB, nBufAB, bufAC, nBufAC, bufABAC, &nBufABAC);
//
//    int bufABABAC[nBufAB + nBufAC];
//    int nBufABABAC;
//    differenceForSortSet(bufAB, nBufAB, bufABAC, nBufABAC, bufABABAC, &nBufABABAC);
//
//    int bufBA[nA + nB + nC];
//    int nBufBA;
//    differenceForSortSet(B, nB, A, nA, bufBA, &nBufBA);
//
//    int bufBC[nA + nB + nC];
//    int nBufBC;
//    differenceForSortSet(B, nB, C, nC, bufBC, &nBufBC);
//
//    int bufBABC[nBufBA + nBufBC];
//    int nBufBABC;
//    differenceForSortSet(bufBA, nBufBA, bufBC, nBufBC, bufBABC, &nBufBABC);
//
//    int bufBABABC[nBufBA + nBufBC];
//    int nBufBABABC;
//    differenceForSortSet(bufBA, nBufBA, bufBABC, nBufBABC, bufBABABC, &nBufBABABC);
//
//    int bufCA[nA + nB + nC];
//    int nBufCA;
//    differenceForSortSet(C, nC, A, nA, bufCA, &nBufCA);
//
//    int bufCB[nA + nB + nC];
//    int nBufCB;
//    differenceForSortSet(C, nC, B, nB, bufCB, &nBufCB);
//
//    int bufCACB[nBufCA + nBufAC];
//    int nBufCACB;
//    differenceForSortSet(bufCA, nBufCA, bufCB, nBufCB, bufCACB, &nBufCACB);
//
//    int bufCACACB[nBufCA + nBufCACB];
//    int nBufCACACB;
//    differenceForSortSet(bufCA, nBufCA, bufCACB, nBufCACB, bufCACACB, &nBufCACACB);
//
//    int bufABABACBABABC[nBufABABAC + nBufBABABC];
//    int nBufABABACBABABC;
//    differenceForSortSet(bufABABAC, nBufABABAC, bufBABABC, nBufBABABC, bufABABACBABABC, &nBufABABACBABABC);
//
//    int buf1[nBufABABACBABABC + nBufBABABC];
//    int nBuf1;
//    symmetricalDifferenceForSortSet(bufABABACBABABC, nBufABABACBABABC, bufBABABC, nBufBABABC, buf1, &nBuf1);
//
//    int buf2[nBuf1 + nBufBABABC];
//    int nBuf2;
//    differenceForSortSet(buf1, nBuf1, bufCACACB, nBufCACACB, buf2, &nBuf2);
//
//    symmetricalDifferenceForSortSet(buf2, nBuf2, bufCACACB, nBufCACACB, res, nRes);
//}
//
//
//int main() {
//    int A[] = {1, 3, 5, 7};
//    int nA = 4;
//    int B[] = {2, 3, 6, 7};
//    int nB = 4;
//    int C[] = {4, 5, 6, 7};
//    int nC = 4;
//
//    outputSet(A, nA, B, nB, C, nC, func1_);
//    printf("\n");
//    outputSet(A, nA, B, nB, C, nC, func2_);
//
//    printf("result is %s", isSetsEquals(A, nA, B, nB, C, nC, func1_, func2_)
//                           ? "equal\n" : "not equal\n");
//
//    return 0;
//}