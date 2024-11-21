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
        n dd 9

        a dd 50
        b dd 100

        f_elem db "%d ", 0Ah, 0 
        f_result db "Result: %d", 0Ah, 0  
.CODE
simple proc
        push ebx ; делитель
        push ecx ; буфер
        push edx ; остаток от деления
        ; 16+

        mov eax, [esp+16]    

        cmp eax, 1
        jle _false

        mov ebx, 1      ; начальный делитель
_cycle: inc ebx         ;    (..следующий)

        mov ecx, ebx    ; проверка на корень
        imul ebx, ebx   ; i^2
        cmp ebx, eax    ; i^2>n
        jg _true
        mov ebx, ecx

        mov ecx, eax    ; проверка на делимость
        xor edx, edx    ; обнулить остаток
        idiv ebx        ; EАХ / EВХ
        or edx, edx     ; остаток нуль?
        je _false        ; да...
        mov eax, ecx
        
        jmp _cycle       ; нет...
 
_true:  mov eax, 1  
        jmp _exit
_false: mov eax, 0  
        jmp _exit
_exit:  pop edx
        pop ecx
        pop ebx
        ret 4
simple endp

enumerate proc
        push eax
        push ebx
        push ecx
        push edx
        ; 20+

        mov ebx, [esp+24] ; a
        mov edx, [esp+20] ; b

        cmp ebx, edx ; a>b
        jg _exit

_cycle: push ebx
        call simple

        cmp eax, 0
        jne _out

_outcb: inc ebx
        cmp ebx, edx
        jge _exit

        jmp _cycle


_out:   push edx
        push ebx
        push offset f_elem
        call crt_printf
        add esp, 4
        pop ebx
        pop edx
        jmp _outcb

_outex: jmp _exit

_exit:  pop edx
        pop ecx
        pop ebx
        pop eax
        ret 

enumerate endp

start:  
        ; push n
        ; call simple

        ; ; Вывод результата в консоль
        ; push eax                   
        ; push offset f_result         
        ; call crt_printf            

        push a
        push b
        call enumerate

        ; Завершаем программу
        push 0
        call ExitProcess

end start