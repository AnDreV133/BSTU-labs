EXTERN printf: PROC

.data 
    fmt db "%d",10, 0

.code

; double* sort(double* res, double* a, int start, int end).
sort_fastcall_args_64  proc
    ; Сохранение регистров
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    push r14
    push r15

    ; Извлечение аргументов из регистров
    mov rdi, rcx        ; Указатель на результирующий массив res (RCX)
    mov rsi, rdx        ; Указатель на исходный массив a (RDX)
    mov rbx, r8        ; Индекс начала start (R8)
    mov rcx, r9        ; Индекс конца end (R9)

    ; Копирование подмассива в res
    sub ecx, ebx        ; Размер подмассива
    inc ecx             ; Увеличиваем на 1, чтобы включить end
    xor edx, edx        ; Индекс в res
copy_loop:
    mov rax, [rsi + rbx * 8] ; Загрузка элемента из a[start]
    mov [rdi + rdx * 8], rax ; Сохранение в res
    inc ebx                  ; Следующий индекс в a
    inc edx                  ; Следующий индекс в res
    loop copy_loop           ; Повторяем для всех элементов

    ; Инициализация FPU
    finit

    ; Сортировка пузырьком
    xor rcx, rcx
    mov ecx, r9d             
    sub ecx, r8d 
    inc ecx

sort_outer_loop:
    push rcx                 ; Сохраняем счетчик внешнего цикла
    xor r13, r13           ; Индекс для внутреннего цикла
sort_inner_loop:

    ; Загрузка текущего и следующего элемента
    fld qword ptr [rdi + r13 * 8] ; Загрузка текущего элемента
    fld qword ptr [rdi + r13 * 8 + 8] ; Загрузка следующего элемента

    ; Сравнение
    fcompp ; Сравнение двух чисел в стеке FPU и выталкивание их из стека
    fstsw ax ; Сохранение слова состояния FPU в AX
    sahf ; Загрузка AH в регистр флагов

    ; Переход, если текущий элемент <= следующего
    jbe no_swap

    ; Обмен значениями
    fld qword ptr [rdi + r13 * 8] ; Загрузка текущего элемента
    fld qword ptr [rdi + r13 * 8 + 8] ; Загрузка следующего элемента
    fstp qword ptr [rdi + r13 * 8] ; Сохранение следующего элемента на место текущего
    fstp qword ptr [rdi + r13 * 8 + 8] ; Сохранение текущего элемента на место следующего

no_swap:
    inc r13d                  ; Следующий индекс
    loop sort_inner_loop      ; Повторяем для всех элементов
    pop rcx                  ; Восстанавливаем счетчик внешнего цикла
    loop sort_outer_loop     ; Повторяем для всех элементов

    ; Восстановление регистров
    pop r15
    pop r14
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx

    ; Возврат указателя на res
    mov rax, rdi
    ret

sort_fastcall_args_64 endp

end