#include <iostream>

#pragma comment(lib, "lab2_c.lib") 
 
extern "C" __declspec(dllimport) double* _stdcall sort_stdcall(double* res, double* a, int start, int end); 
extern "C" __declspec(dllimport) double* _cdecl sort_cdecl(double* res, double* a, int start, int end); 
extern "C" __declspec(dllimport) double* _fastcall sort_fastcall(double* res, double* a, int start, int end); 
extern "C" __declspec(dllimport) double* _stdcall sort_stdcall_args(double* res, double* a, int start, int end); 
extern "C" __declspec(dllimport) double* _cdecl sort_cdecl_args(double* res, double* a, int start, int end);

// Функция сортировки пузырьком
double* sort(double* res, double* a, int start, int end) {
    // Копируем элементы из исходного массива a в res в диапазоне [start, end]
    for (int i = start; i <= end; ++i) {
        res[i - start] = a[i];
    }

    // Сортировка пузырьком
    for (int i = start; i <= end; ++i) {
        for (int j = start; j < end - (i - start); ++j) {
            if (res[j - start] > res[j + 1 - start]) {
                // Обмен значениями
                double temp = res[j - start];
                res[j - start] = res[j + 1 - start];
                res[j + 1 - start] = temp;
            }
        }
    }

    // Возвращаем указатель на отсортированный массив
    return res;
}

// Пример использования
int main() {
    double a[] = {4.5, 2.3, 7.1, 3.8, 1.2, 6.4, 5.0};
    int start = 2;
    int end = 5;
    int size = end - start + 1; // Размер подмассива

    // Резервируем память для результирующего массива
    double* res = new double[size];

    // Вызываем функцию сортировки
    sort_stdcall_args(res, a, start, end);

    // Вывод отсортированного подмассива
    std::cout << "Sorted subarray: ";
    for (int i = 0; i < size; ++i) {
        std::cout << res[i] << " ";
    }
    std::cout << std::endl;

    // Освобождаем память
    delete[] res;

    return 0;
}