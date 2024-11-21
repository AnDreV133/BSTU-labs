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

.DATA
	
    h db 4
    x db 1,2,3,4
    y dw 4,3,2,1
    n dw 4

	format db "Result: %d", 0Ah, 0  

.CODE

f proc
    push ebx
    push ecx ; 12+

    xor eax, eax
    mov ax, [esp+14]

    cmp eax, 1          ; x > 1
    ja f_cond1			  
    jmp f_func2

f_cond1:
    cmp eax, 7          ; x < 7
    jl f_func1		
    jmp f_func2	

f_func1:
    mov ebx, eax
    imul ebx, ebx       ; x^2
    push ebx

    xor ebx, ebx
    mov bx, [esp+16]

    mov ecx, eax
    mov eax, ebx
    dec ecx
    f_l: 
        imul ebx, eax   ; y^x
    loop f_l

    pop eax
    sub eax, ebx        ; x^2 - y^x
    
    jmp f_end_func

f_func2:
    push eax

    imul eax, eax       ; x^2

    xor ebx, ebx
    mov bx, [esp+16]
    push ebx

    imul ebx, ebx       ; y^2

    add eax, ebx

    pop ebx
    pop ecx

    imul ebx, ecx
    xchg eax, ebx
    push eax
    call crt_abs        ; |xy|

    add eax, ebx

    pop ebx

    jmp f_end_func

f_end_func:
    pop ecx
    pop ebx
    ret 4
f endp

start:

    xor esi, esi

    ; Вычисление суммы
    movsx ecx, n
    mov ebx, 0 ; инициализация суммы
    mov eax, 0 ; для временного хранения результата

sum_loop:
    push ebx

    ; Вычисляем i / h
    mov eax, esi
    movsx ebx, h
    mov edx, 0
    idiv ebx
    
    ; Умножаем 100 * (i / h)
    mov ebx, 100
    imul eax, ebx

    ; Добавляем к сумме
    pop ebx
    add ebx, eax

    push ebx

    ; Вычисляем y[i] / x[i]
    movsx eax, y[esi*2]
    mov edx, 0
    movsx ebx, x[esi]
    idiv ebx

    pop ebx
    
    ; Добавляем y[i] / x[i] к сумме
    add ebx, eax

    ; Вызываем функцию
    xor ax, ax
    mov al, x[esi]
    push ax
    push y[esi*2]
    call f 

    add ebx, eax

    inc esi
    loop sum_loop


    ; Вывод результата в консоль
    push ebx
    push offset format         
    call crt_printf            

    ; Завершаем программу
    push 0
    call ExitProcess

end start