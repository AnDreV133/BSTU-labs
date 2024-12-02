.386
.model flat, stdcall
option casemap:none

include \masm32\include\masm32.inc
include \masm32\include\kernel32.inc
include \masm32\include\msvcrt.inc

includelib \masm32\lib\masm32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\msvcrt.lib

.data
    fmtF db "%d", 0 
    fmtNewLine db 10, 13, 0
    number db 20 dup(0) ; 20 байт для большого числа
    k dd 3              ; Значение k
    fmt db "k = %d", 13, 10, 0

.code
start:
    ; Инициализация большого числа
    mov dword ptr [number], 0x12345678
    mov dword ptr [number + 4], 0x9ABCDEF0
    mov dword ptr [number + 8], 0x11223344
    mov dword ptr [number + 12], 0x55667788
    mov dword ptr [number + 16], 0x99AABBCC

    ; Вывод значения k
    push k
    push offset fmt
    call crt_printf
    add esp, 8

    ; Вызов процедуры деления
    push k
    push offset number
    call divide_big_number
    add esp, 8

    ; Вывод результата
    push offset number
    call output_big_number
    add esp, 4

    ; Завершение программы
    invoke ExitProcess, 0

divide_big_number proc uses eax ebx ecx edx esi edi, number_: dword, k_: dword
    mov esi, number_
    mov ecx, k_

    ; Проверка знака числа
    mov al, [esi + 19] ; Загружаем старший байт (знаковый бит)
    and al, 0x80       ; Проверяем, установлен ли старший бит
    jz positive_number ; Если не установлен, число положительное

    ; Если число отрицательное, устанавливаем все биты в 1
    mov edi, esi
    mov ebx, 20
set_negative_bits:
    mov byte ptr [edi], 0xFF
    inc edi
    dec ebx
    jnz set_negative_bits

positive_number:
    ; Выполняем сдвиг вправо на k бит
    mov ebx, ecx       ; Загружаем k в ebx

shift_loop:
    ; Сдвигаем каждый байт вправо на 1 бит
    mov edi, 20        ; Количество байтов
shift_byte:
    shr byte ptr [esi], 1  ; Сдвигаем текущий байт вправо
    rcr byte ptr [esi + 1], 1 ; Сдвигаем следующий байт с учетом переноса
    inc esi
    dec edi
    jnz shift_byte

    ; Уменьшаем счетчик сдвигов
    dec ebx
    jnz shift_loop

    ret
divide_big_number endp

output_big_number proc uses eax ebx ecx edx esi, number_: dword
    mov esi, number_
    mov ecx, 20

output_loop:
    movzx eax, byte ptr [esi]
    push ecx
    push eax
    push offset fmtF
    call crt_printf
    add esp, 8
    pop ecx
    inc esi
    loop output_loop

    ; Вывод новой строки
    push offset fmtNewLine
    call crt_printf
    add esp, 4

    ret
output_big_number endp

end start