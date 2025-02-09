#include <windows.h>
#include <DbgHelp.h>
#include <stdio.h>
#include <locale.h>

#pragma comment(lib, "dbghelp.lib")

// ������ � ���������:
// https://stackoverflow.com/questions/22465253/symgetlinefromaddr-not-working-properly

void print_trace(LPVOID instructionAddress)
{
	printf("����������� ����� ������� � ������, �������� ����������� ���������� � ������� 0x%p\n", instructionAddress);
	DWORD flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;
	
	HMODULE hModule; // ���������� ������

	// ��������� ���������� ������ ������ �� ������ ���������� instructionAddress
	if (GetModuleHandleExA(flags, (LPCSTR)instructionAddress, &hModule))
	{
		char module_name[MAX_PATH + 1];

		// ��������� ����� ����������
		GetModuleFileNameA(hModule, module_name, MAX_PATH);
		printf("��������� ����� ������: 0x%p\n��� ������: %s\n", hModule, module_name);

		DWORD64 disp = 0; // �������� ������������ ������ ������������
		// ��������� ������� SymFromAddr ���������� ��� ��������� ��������� � ����� ��������� SYMBOL_INFO, �����
		// ����� ����� �� ����������� ����� ��� ���������� �����
		struct
		{
			IMAGEHLP_SYMBOL64 symbolInfo = { };
			char name_buffer[1024];

		} SYMBOL_DATA;
		
		SYMBOL_DATA.symbolInfo.SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64); // ������ ���������
		SYMBOL_DATA.symbolInfo.MaxNameLength = 1024;	// ����� ������ ��� ���������� ����������� �����

		//decltype(SymGetSymFromAddr64)* pSymGetSymFromAddr64 = (decltype(SymGetSymFromAddr64)*)(GetProcAddress(LoadLibraryA("dbghelp.dll"), "SymGetSymFromAddr64"));

		// ��������� ����� �������, ������� �������� ���������� � ������� instructionAddress
		if (SymGetSymFromAddr64(GetCurrentProcess(), (DWORD64)instructionAddress, &disp,
			&SYMBOL_DATA.symbolInfo))
			 //PUSH offset SD
			 //PUSH offset disp
			 //PUSH 0
			 //PUSH instructionAddress
			 //PUSH hProcess
			 //CALL pSymFromAddr
		{
			printf("��� ������������: %s, �������� ���������� � ������ �� ������ �������: %llu\n", SYMBOL_DATA.symbolInfo.Name, disp);

			// ��������� ������ ������ � �������� ������ ���������
			// ����� ����� ���� �������� ����� ������, ���������� ������ ���� ���������� /LTCG:incremental � ���������� �������:
			// ����������� -> ����������� -> �������� ���� �� ����� ���������� -> ������������ �������� ���� �� ����� ���������� (/LTCG)
			IMAGEHLP_LINE64 line;
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			DWORD displacement;
			if (SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)instructionAddress, &displacement, &line))
			{
				printf("������������ \"%s\" ��������� � ����� \"%s\"\n�����: 0x%p\n�������� ���������� � ������ �� ������ ������: %d\n������: %d\n", SYMBOL_DATA.symbolInfo.Name, line.FileName, (DWORD_PTR)SYMBOL_DATA.symbolInfo.Address, displacement, line.LineNumber);
			}
			else
			{
				printf("SymGetLineFromAddr64 ���������� ��� ������: %d\n", GetLastError());
			}
		}
		else
		{
			printf("SymGetSymFromAddr64 ���������� ��� ������: %d\n", GetLastError());
		}
	}
}

void main(void)
{
	setlocale(LC_ALL, "Russian");

	//SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES);
	
	SymInitialize(GetCurrentProcess(), NULL, TRUE);

	//LPVOID some_instruction = (LPVOID)((DWORD_PTR)&GetTickCount + __rdtsc() % 10000); // ���������� �� ��������� �������

	//print_trace(some_instruction); // ����� �� ��������� �������

	typedef USHORT(WINAPI* CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);

	CaptureStackBackTraceType pCaptureStackBackTraceType = (CaptureStackBackTraceType)(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));

	if (pCaptureStackBackTraceType == NULL)
		return;

	void* callers[1024];
	int count = pCaptureStackBackTraceType(0, _countof(callers), callers, NULL);
	for (int i = 0; i < count; i++) // ��������� ��� ������ �������� � �����
	{
		printf("------- ������� %d, ����� 0x%p ------- \n", i + 1, callers[i]);
		print_trace(callers[i]);
		puts("\n");
	}
}