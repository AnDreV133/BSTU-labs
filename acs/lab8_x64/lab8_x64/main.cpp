#include <iostream>

extern int printf(char const* const _Format, ...);

// Как создать 32-разрядный ассемблерный код в отдельном asm-файле в Visual Studio описано здесь:
// https://habr.com/ru/post/111275/

// Основы написания 64-разрядного ассемблерного кода в Visual Studio на ассемблере:
// https://lallouslab.net/2016/01/11/introduction-to-writing-x64-assembly-in-visual-studio/

// Возвращает сумму чисел: a + b + c + d + e
// Стиль вызова - fastcall
extern "C" long long sum_numbers_fastcall(long long a, long long b, long long c, long long d, long long e);

// Функция с переменным числом аргументов
// Возвращает сумму из count 8-байтовых чисел, перечисляемых после первого аргумента
// Стиль вызова - fastcall
extern "C" long long sum_args_fastcall(int count, ...);

// Печать в консоль массива 4-байтовых целых чисел. count - длина массива
// Стиль вызова - fastcall
extern "C" void print_array(int count, int* elements);

int main(void) 
{
	setlocale(LC_ALL, "Russian");

	std::cout << "Функция с переменным числом аргументов. Сумма аргументов - " << sum_args_fastcall(7, 1000ll, 20ll, 0ll, 5ll, 6ll, 7ll, 8ll) << std::endl;

	std::cout << "Функция в стиле fastcall. Сумма аргументов - " << sum_numbers_fastcall(1, 2, 0, 0, 5) << std::endl;

	std::cout << "Массив целых чисел:" << std::endl;
	int elements[] = { 1, 2, 3, -1 };	
	print_array(4, elements);
}
