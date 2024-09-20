.686				
 .model flat, stdcall
 option casemap: none

;  include windows.inc
;  include kernel32.inc
;  include msvcrt.inc
;  includelib	kernel32.lib
;  includelib	msvcrt.lib

.DATA
	strt DB "Some string", 0
	DW 400
	a DF 900
	mas DD 4*8 DUP (3)
	s DQ 15.7
.CODE
START:
	MOV EAX, mas[0]
	MOV EBX, DWORD PTR [a]
	SUB EBX, EAX
	INC EBX
	MOV mas[4], EBX

	; push offset strt
	; call crt_puts
	; add ESP, 4

	; call crt__getch

	; push 0
	; call ExitProcess
END START
