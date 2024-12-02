.686			
.model flat, stdcall
option casemap: none

include windows.inc
include kernel32.inc
include msvcrt.inc
include user32.inc

includelib kernel32.lib
includelib msvcrt.lib
includelib user32.lib

.data
    fmtF db "%d", 0 
    fntNewLine db 10, 13, 0

    number dw 15 dup(7777h), 07531h

    fmt db "k = %d", 13, 10, 0

.code
output proc uses eax ebx edx ecx a_ : word
    xor eax, eax 
    xor edx, edx 
    mov ax, a_ 
    clc
    mov ecx, 16

output_number: 
    rol ax, 1 
    mov edx, eax 
    and edx, 1b
    push eax 
    push ecx 
    push edx

    push edx
    push offset fmtF 
    call crt_printf

    add esp, @WordSize * 2

    pop edx 
    pop ecx
    pop eax
    loop output_number

    ret 
output endp

output_big_number proc uses eax ebx edx ecx number_ : dword 
    mov ebx, number_

    mov ecx, 0 
print_val1_:
    invoke output, word ptr [ebx + ecx * 2]

    inc ecx 
    cmp ecx, 16
    jne print_val1_
    invoke crt_printf, offset fntNewLine 
    add esp, @WordSize

    ret 
output_big_number endp

div_2 proc number_ptr: dword, k_ : dword 
    pushad

    mov ecx, k_
    mov ebx, number_ptr

loop_start:
    push ecx
    mov esi, ebx

    clc
    mov ecx, 16

loop_shift:
    rcr word ptr [esi], 1 
    pushf
    add esi, 2 
    popf
    loop loop_shift

    pop ecx 
    loop loop_start

    popad
    ret 
div_2 endp

START:
invoke output_big_number, offset number 

invoke div_2, offset number, 2
invoke output_big_number, offset number 

invoke div_2, offset number, 1
invoke output_big_number, offset number

invoke div_2, offset number, 1
invoke output_big_number, offset number 

push 0	
call ExitProcess	

END START
