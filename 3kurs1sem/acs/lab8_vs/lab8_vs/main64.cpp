//#include <iostream> 
//#include <vector> 
//#include <assert.h> 
//#include <chrono> 
//
//
////extern "C" long double* sort_fastcall_args_64();
//extern int printf(char const* const _Format, ...);
//extern "C" long double* sort_fastcall_args_64(long double* res, long double* a, long long start, long long end);
//
//
//
//// // Пример использования
////int main() {
////    long double a[] = {4.5, 2.3, 7.1, 3.8, 1.2, 6.4, 5.0};
////    int start = 2;
////    int end = 5;
////    int size = end - start + 1; // Размер подмассива
////
////    // Резервируем память для результирующего массива
////    long double* res = new long double[size];
////
////    // Вызываем функцию сортировки
////    sort_fastcall_args_64(res, a, start, end);
////
////    // Вывод отсортированного подмассива
////    std::cout << "Sorted subarray: ";
////    for (int i = 0; i < size; ++i) {
////        std::cout << res[i] << " ";
////    }
////    std::cout << std::endl;
////
////    // Освобождаем память
////    delete[] res;
////
////    return 0;
////}
//
//
//template <typename TestedFunction>
//void test_function(TestedFunction func_to_test, int amount) {
//    long double* a = (long double*)malloc(sizeof(long double) * amount);
//    long double* res = (long double*)malloc(sizeof(long double) * (amount - 2));
//
//    if (a == NULL || res == NULL) return;
//
//    for (int i = 0; i < amount; i++) {
//        a[i] = (long double)(rand() % 1000);
//    }
//
//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    func_to_test(res, a, 1, amount - 2);
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
//
//    std::cout << delta / 1000.0 << std::endl;
//    free(a);
//    //free(res);
//}
//
//int main() {
//    int amount = 1000;
//    for (size_t i = 0; i < 10; i++) {
//        std::cout << "for amount: " << amount << std::endl;
//
//        std::cout << "  fastcall_64 function: ";
//        test_function(sort_fastcall_args_64, amount);
//
//        std::cout << "\n";
//        amount += 2500;
//    }
//    return 0;
//}