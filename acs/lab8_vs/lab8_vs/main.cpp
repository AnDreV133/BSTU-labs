//#include <iostream> 
//#include <vector> 
//#include <assert.h> 
//#include <chrono> 
//
//#pragma comment(lib, "lab2_c.lib") 
// 
//extern "C" __declspec(dllimport) long double* _stdcall sort_stdcall(long double* res, long double* a, int start, int end);
//extern "C" __declspec(dllimport) long double* _cdecl sort_cdecl(long double* res, long double* a, int start, int end);
//extern "C" __declspec(dllimport) long double* _fastcall sort_fastcall(long double* res, long double* a, int start, int end);
//extern "C" __declspec(dllimport) long double* _stdcall sort_stdcall_args(long double* res, long double* a, int start, int end);
//extern "C" __declspec(dllimport) long double* _cdecl sort_cdecl_args(long double* res, long double* a, int start, int end);
//
//// Функция сортировки пузырьком
//long double* sort(long double* res, long double* a, int start, int end) {
//    // Копируем элементы из исходного массива a в res в диапазоне [start, end]
//    for (int i = start; i <= end; ++i) {
//        res[i - start] = a[i];
//    }
//
//    // Сортировка пузырьком
//    for (int i = start; i <= end; ++i) {
//        for (int j = start; j < end - (i - start); ++j) {
//            if (res[j - start] > res[j + 1 - start]) {
//                // Обмен значениями
//                double temp = res[j - start];
//                res[j - start] = res[j + 1 - start];
//                res[j + 1 - start] = temp;
//            }
//        }
//    }
//
//    // Возвращаем указатель на отсортированный массив
//    return res;
//}
//
//// Пример использования
////int main() {
////    double a[] = {4.5, 2.3, 7.1, 3.8, 1.2, 6.4, 5.0};
////    int start = 2;
////    int end = 5;
////    int size = end - start + 1; // Размер подмассива
////
////    // Резервируем память для результирующего массива
////    double* res = new double[size];
////
////    // Вызываем функцию сортировки
////    sort_fastcall(res, a, start, end);
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
//    for (int i = 0; i < amount; i++) {
//        a[i] = (long double) (rand() % 1000);
//    }
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
//        std::cout << "  stdcall function: "; 
//        test_function(sort_stdcall, amount);
//        
//        std::cout << "  cdecl function: "; 
//        test_function(sort_cdecl, amount);
//        
//        std::cout << "  fastcall function: "; 
//        test_function(sort_fastcall, amount);
//        
//        std::cout << "  stdcall whit args function: "; 
//        test_function(sort_stdcall_args, amount);
//        
//        std::cout << "  cdecl whit args function: "; 
//        test_function(sort_cdecl_args, amount);
//        
//        std::cout << "  c++ function: "; 
//        test_function(sort, amount);
//        
//        std::cout << "\n";
//        amount += 2500;
//    }
//    return 0;
//}