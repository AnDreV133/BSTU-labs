.686
.model flat, stdcall
option casemap:none

include \masm32\include\masm32.inc
include \masm32\include\kernel32.inc
include \masm32\include\msvcrt.inc

includelib \masm32\lib\masm32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\msvcrt.lib

.data
    fmtBit db "%d", 0        ; Формат для вывода одного бита
    fmtSpace db " ", 0       ; Формат для вывода пробела
    fmtNewLine db 10, 13, 0
    
    ; number db 07Fh, 19 dup(0FFh) 
    ; number db 19 dup(0FFh), 0FFh 
    ; number db 19 dup(000h), 010h 
    number db 010h, 19 dup(000h) 
    ; number db 18 dup(000h), 001h, 000h

    k dd 3              ; Значение k
    fmt db "k = %d", 13, 10, 0

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

; divide_big_number proc uses eax ebx ecx edx esi edi, number_: dword, k_: dword
;     mov esi, number_
;     mov ecx, k_

;     ; Проверка знака числа
;     mov al, [esi + 19] ; Загружаем старший байт (знаковый бит)
;     and al, 080h        ; Проверяем, установлен ли старший бит
;     jz positive_number ; Если не установлен, число положительное

;     ; Если число отрицательное, устанавливаем все биты в 1
;     mov edi, esi
;     mov ebx, 20
; set_negative_bits:
;     mov byte ptr [edi], 0FFh
;     inc edi
;     dec ebx
;     jnz set_negative_bits

; positive_number:
;     ; Выполняем сдвиг вправо на k бит
;     mov ebx, ecx       ; Загружаем k в ebx

; shift_loop:
;     ; Сдвигаем каждый байт вправо на 1 бит
;     mov edi, 20        ; Количество байтов
; shift_byte:
;     shr byte ptr [esi], 1  ; Сдвигаем текущий байт вправо
;     rcr byte ptr [esi + 1], 1 ; Сдвигаем следующий байт с учетом переноса
;     inc esi
;     dec edi
;     jnz shift_byte

;     ; Уменьшаем счетчик сдвигов
;     dec ebx
;     jnz shift_loop

;     ret
; divide_big_number endp


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


; output_big_number proc uses eax ebx ecx edx esi, number_: dword
;     mov esi, number_
;     mov ecx, 20

; output_loop:
;     movzx eax, byte ptr [esi]
;     push ecx
;     push eax
;     push offset fmtF
;     call crt_printf
;     add esp, 8
;     pop ecx
;     inc esi
;     loop output_loop

;     ; Вывод новой строки
;     push offset fmtNewLine
;     call crt_printf
;     add esp, 4

;     ret
; output_big_number endp

start:
    ; Вывод значения k
    push k
    push offset fmt
    call crt_printf
    add esp, 8

    ; Вывод исходного числа
    invoke output_big_number, offset number

    ; Вызов процедуры деления
    push k
    push offset number
    call div_2
    add esp, 8

    ; Вывод результата
    push offset number
    call output_big_number
    add esp, 4

    ; Завершение программы
    invoke ExitProcess, 0

end start