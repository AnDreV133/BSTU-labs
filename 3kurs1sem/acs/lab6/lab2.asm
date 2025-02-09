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
    fmtBit db "%d", 0        ; Формат для вывода одного бита
    fmtSpace db " ", 0       ; Формат для вывода пробела

    fmtF db "%d", 0 
    fmtNewLine db 10, 13, 0

    number db 19 dup(77h), 000h
    ; number db 19 dup(0FFh), 0F8h

.code
output_big_number proc uses eax ebx ecx edx esi, number_: dword
    mov esi, number_
    ; add esi, 19
    mov ecx, 20

output_loop:
    push ecx                  ; Сохраняем ecx
    mov ecx, 8                ; Устанавливаем счетчик для 8 бит
    mov ebx, 010000000b                ; Маска для выделения бита

bit_loop:
    dec ecx

    mov eax, [esi]            ; Загружаем текущий байт в eax
    and eax, ebx              ; Выделяем текущий бит
    shr eax, cl
    push ecx                  ; Сохраняем ecx
    push eax                  ; Передаем бит в качестве аргумента
    push offset fmtBit        ; Передаем формат для вывода бита
    call crt_printf           ; Вызываем функцию printf
    add esp, 8                ; Восстанавливаем стек
    pop ecx                   ; Восстанавливаем ecx

    shr ebx, 1                ; Сдвигаем маску для следующего бита

    cmp ecx, 0
    jne bit_loop             ; Повторяем цикл для оставшихся бит

    push offset fmtSpace      ; Передаем формат для вывода пробела
    call crt_printf           ; Вызываем функцию printf
    add esp, 4                ; Восстанавливаем стек

    pop ecx                   ; Восстанавливаем ecx
    inc esi                   ; Переходим к следующему байту
    loop output_loop          ; Повторяем цикл для оставшихся байтов

    ; Вывод новой строки
    push offset fmtNewLine    ; Передаем формат для вывода новой строки
    call crt_printf           ; Вызываем функцию printf
    add esp, 4                ; Восстанавливаем стек

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
    mov ecx, 20

loop_shift:
    sar byte ptr [esi], 1 
    pushf
    add esi, 1 
    popf
    loop loop_shift

    pop ecx 
    loop loop_start

    popad
    ret 
div_2 endp

START:
invoke output_big_number, offset number 

; invoke div_2, offset number, 2
; invoke output_big_number, offset number 

; invoke div_2, offset number, 1
; invoke output_big_number, offset number

invoke div_2, offset number, 10
invoke output_big_number, offset number 

push 0	
call ExitProcess	

END START
