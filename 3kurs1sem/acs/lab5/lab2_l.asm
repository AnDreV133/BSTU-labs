.386
.model flat, stdcall
option casemap :none

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\msvcrt.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\msvcrt.lib

.data
    fmt db "n = %d, res = %0.6f", 0Ah, 0  ; Формат строки для вывода
    n dd 1                                ; Начальное значение n
    max_n dd 50                           ; Максимальное значение n
    q real8 9.869604401089358             ; pi^2
    res real8 0.0                         ; Переменная для хранения результата

.code
start:
    finit                   ; Инициализация FPU

    mov ecx, n              ; Загружаем начальное значение n
    fld q                   ; Загружаем q (pi^2) в стек FPU

main_loop:
    cmp ecx, max_n          ; Проверяем, достигли ли мы максимального значения n
    jg done                 ; Если да, завершаем программу

    ; Вычисляем (2n + 1)
    fild ecx    ; Загружаем n в стек FPU
    fadd st(0), st(0)       ; Умножаем n на 2
    fld1                    ; Загружаем 1 в стек FPU
    faddp st(1), st(0)      ; Добавляем 1 к 2n, получаем (2n + 1)

    ; Вычисляем (2n^2 - 1)
    fild ecx    ; Загружаем n в стек FPU
    fmul st(0), st(0)       ; Возводим n в квадрат
    fadd st(0), st(0)       ; Умножаем на 2
    fld1                    ; Загружаем 1 в стек FPU
    fsubp st(1), st(0)      ; Вычитаем 1 из 2n^2, получаем (2n^2 - 1)

    ; Вычисляем (2n + 1) / (2n^2 - 1)
    fdivp st(1), st(0)      ; Делим (2n + 1) на (2n^2 - 1)

    ; Вычисляем 1 - (2n + 1) / (2n^2 - 1)
    fld1                    ; Загружаем 1 в стек FPU
    fsubp st(1), st(0)      ; Вычитаем результат из 1

    ; Вычисляем lg(1 - (2n + 1) / (2n^2 - 1))
    fldlg2                  ; Загружаем lg(2) в стек FPU
    fxch st(1)              ; Меняем местами st(0) и st(1)
    fyl2x                   ; Вычисляем lg(x) = lg(2) * log2(x)

    ; Сохраняем результат в переменную res
    fstp res                ; Сохраняем результат в res

    ; Выводим результат с помощью crt_printf
    invoke crt_printf, addr fmt, ecx, res

    ; Переход к следующему n
    inc ecx
    jmp main_loop

done:
    invoke ExitProcess, 0

end start