#include <iostream>

extern int printf(char const* const _Format, ...);

// ��� ������� 32-��������� ������������ ��� � ��������� asm-����� � Visual Studio ������� �����:
// https://habr.com/ru/post/111275/

// ������ ��������� 64-���������� ������������� ���� � Visual Studio �� ����������:
// https://lallouslab.net/2016/01/11/introduction-to-writing-x64-assembly-in-visual-studio/

// ���������� ����� �����: a + b + c + d + e
// ����� ������ - fastcall
extern "C" long long sum_numbers_fastcall(long long a, long long b, long long c, long long d, long long e);

// ������� � ���������� ������ ����������
// ���������� ����� �� count 8-�������� �����, ������������� ����� ������� ���������
// ����� ������ - fastcall
extern "C" long long sum_args_fastcall(int count, ...);

// ������ � ������� ������� 4-�������� ����� �����. count - ����� �������
// ����� ������ - fastcall
extern "C" void print_array(int count, int* elements);

int main(void) 
{
	setlocale(LC_ALL, "Russian");

	std::cout << "������� � ���������� ������ ����������. ����� ���������� - " << sum_args_fastcall(7, 1000ll, 20ll, 0ll, 5ll, 6ll, 7ll, 8ll) << std::endl;

	std::cout << "������� � ����� fastcall. ����� ���������� - " << sum_numbers_fastcall(1, 2, 0, 0, 5) << std::endl;

	std::cout << "������ ����� �����:" << std::endl;
	int elements[] = { 1, 2, 3, -1 };	
	print_array(4, elements);
}
