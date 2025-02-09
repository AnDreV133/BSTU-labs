EXTERN printf: PROC

.DATA
   fmt_int db "%d ", 0
.CODE

sum_numbers_fastcall PROC
    MOV RAX, RCX    ; Первый аргумент находится в RCX
    ADD RAX, RDX    ; Второй аргумент находится в RDX
    ADD RAX, R8     ; Третий аргумент находится в R8
    ADD RAX, R9     ; Четвёртый аргумент находится в R9
    ADD RAX, [RSP + 5*8]    ; Остальные аргументы находятся в стеке, начиная с адреса RSP + 40
    ; 40 байт - это теневое хранилище для 4 регистров и адреса возврата
    RET
sum_numbers_fastcall ENDP 

sum_args_fastcall PROC
    PUSH RSI
    XOR RSI, RSI    ; RSI = 0
    XOR RAX, RAX    ; RAX = 0

    ; ECX - общее количество суммируемых чисел
    CMP ECX, 0      ; Если ECX <= 0, то возврат RAX
    JLE exit_label        
    ADD RAX, RDX    ; RAX += RDX ; Прибавляем первый аргумент
    INC ESI         ; ESI++

    CMP ECX, 1      ; Если ECX <= 1, то возврат RAX
    JLE exit_label        
    ADD RAX, R8     ; RAX += R8 ; Прибавляем второй аргумент
    INC ESI         ; ESI++

    CMP ECX, 2      ; Если ECX <= 2, то возврат RAX
    JLE exit_label        
    ADD RAX, R9     ; RAX += R9  ; Прибавляем третий аргумент
    INC ESI         ; ESI++

    ; Суммируем оставшиеся в стеке элементы
add_loop:
    CMP ESI, ECX    ; Пока ESI < ECX
    JGE exit_label
    ADD RAX, [RSP + RSI*8 + 24]
    INC ESI         ; ESI++
    JMP add_loop

exit_label:
    POP RSI
    RET

sum_args_fastcall ENDP


print_array PROC
    ; В RCX хранится количество элементов в массиве
    ; В RDX - указатель на массив
    PUSH RSI
    MOV RSI, RDX

print_loop:
    JECXZ exit_print ; Если ECX=0, то конец алгоритма

    PUSH RCX     ; Сохраним RCX в стеке, потому что функция printf изменяет его
    SUB RSP, 5*8 ; Выделение памяти для теневого хранилища
    MOV RCX, offset fmt_int
    MOV RDX, [RSI]
    call printf ; printf("%d ", RDX)
    ADD RSP, 5*8
    POP RCX

    ADD RSI, 4 ; Переход к следующему элементу в массиве
    DEC ECX    ; Уменьшение количества оставшихся элементов
    JMP print_loop

exit_print:
    POP RSI
    RET
print_array ENDP

END 