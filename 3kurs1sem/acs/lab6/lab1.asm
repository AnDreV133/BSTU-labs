.686
.model flat, stdcall	
option casemap: none	

include c:\masm32\include\windows.inc 
include c:\masm32\include\kernel32.inc 
include c:\masm32\include\user32.inc 
include c:\masm32\include\msvcrt.inc 
include c:\masm32\include\masm32rt.inc

includelib c:\masm32\lib\kernel32.lib 
includelib c:\masm32\lib\msvcrt.lib
includelib c:\masm32\lib\user32.lib

.data 
	mask1 dd 1b
	fmt db "%d", 0 
	fntNewLine db 10,13, 0
	fmt_number db "%d :", 9, 0

.code
output proc uses eax ebx edx ecx a_ : dword 
	mov eax, a_
	
	shl eax, 12 
	clc

mov ecx, 20 
output_number: 
	rol eax, 1
	mov edx, eax 
	and edx, 1b

	push eax 
	push ecx 
	push edx

	push edx
	push offset fmt 
	call crt_printf

	add esp, @WordSize * 2

	pop edx 
	pop ecx 
	pop eax
	loop output_number

	push offset fntNewLine 
	call crt_printf

	add esp, @WordSize

	ret
output endp


START:
	mov ecx, 19 
	mov esi, 1

first_loop: 
	push ecx

	mov ebx, mask1 
	shl ebx, cl

second_loop: 
	dec ecx
	mov edx, mask1 
	shl edx, cl

	xor eax, eax
	not eax

	xor eax, ebx 
	xor eax, edx

	push ecx 
	push eax 
	push ebx 
	push esi
	push offset fmt_number 
	call crt_printf
	add esp, @WordSize 
	pop esi
	inc esi 
	pop ebx 
	pop eax 
	pop ecx

	invoke output, eax

	cmp ecx, 0
	jne second_loop

	pop ecx
	loop first_loop

	push 0  
	call ExitProcess	

END START
