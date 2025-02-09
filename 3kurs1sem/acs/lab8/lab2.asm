.686
.model flat, stdcall	
option casemap: none	

include windows.inc 
include kernel32.inc 
include msvcrt.inc 
includelib  kernel32.lib 
includelib  msvcrt.lib 

.data 

    ; Исходный массив a
    a dq 4.5, 2.3, 7.1, 3.8, 1.2, 6.4, 5.0

    ; Результирующий массив res
    res dq 0.0, 0.0, 0.0, 0.0

    ; Индексы start и end
    start_ dd 2
    end_ dd 5

    ; Строка формата для вывода чисел
    format db "%.2f ", 0
    newline db 13, 10, 0 ; Символы новой строки

.code

; double* sort(double* res, double* a, int start, int end).
sort_stdcall proc
    ; Сохранение регистров
    push ebp
    mov ebp, esp
    add ebp, 8
    push ebx
    push esi
    push edi
    push ecx
    push edx

    ; Извлечение аргументов из стека
    mov edi, [ebp]  ; Указатель на результирующий массив res
    mov esi, [ebp + 4] ; Указатель на исходный массив a
    mov ebx, [ebp + 8] ; Индекс начала start
    mov ecx, [ebp + 12] ; Индекс конца end

    ; Копирование подмассива в res
    sub ecx, ebx        ; Размер подмассива
    inc ecx             ; Увеличиваем на 1, чтобы включить end
    mov edx, 0          ; Индекс в res
copy_loop:
    push ecx
    mov eax, [esi + ebx * 8] ; Загрузка элемента из a[start]
    mov ecx, [esi + ebx * 8 + 4]
    mov [edi + edx * 8], eax ; Сохранение в res
    mov [edi + edx * 8 + 4], ecx 
    pop ecx
    inc ebx                  ; Следующий индекс в a
    inc edx                  ; Следующий индекс в res
    loop copy_loop           ; Повторяем для всех элементов

    finit
    ; Сортировка пузырьком
    mov ecx, [ebp + 12]
    sub ecx, [ebp + 8]      ; Размер подмассива
    inc ecx                  ; Увеличиваем на 1
sort_outer_loop:
    push ecx                 ; Сохраняем счетчик внешнего цикла
    mov edx, 0               ; Индекс для внутреннего цикла
sort_inner_loop:
    fld qword ptr [edi + edx * 8] ; Загрузка числа с плавающей точкой в стек FPU
    fld qword ptr [edi + edx * 8 + 8] ; Загрузка следующего числа с плавающей точкой в стек FPU
    fcompp ; Сравнение двух чисел в стеке FPU и выталкивание их из стека
    fstsw ax ; Сохранение слова состояния FPU в AX
    sahf ; Загрузка AH в регистр флагов
    jbe no_swap ; Переход, если текущий элемент <= следующего
    
    fld qword ptr [edi + edx * 8] ; Обмен значениями
    fld qword ptr [edi + edx * 8 + 8]
    fstp qword ptr [edi + edx * 8]
    fstp qword ptr [edi + edx * 8 + 8]
no_swap:
    inc edx                  ; Следующий индекс
    loop sort_inner_loop     ; Повторяем для всех элементов
    pop ecx                  ; Восстанавливаем счетчик внешнего цикла
    loop sort_outer_loop     ; Повторяем для всех элементов

    ; Восстановление регистров
    pop edx
    pop ecx
    pop edi
    pop esi
    pop ebx

    ; Возврат указателя на res
    mov eax, [ebp]
    pop ebp
    ret 16                  ; Очистка стека (4 аргумента * 4 байта)
sort_stdcall endp


; double* sort(double* res, double* a, int start, int end).
sort_cdecl proc
    ; Сохранение регистров
    push ebp
    mov ebp, esp
    add ebp, 8
    push ebx
    push esi
    push edi
    push ecx
    push edx

    ; Извлечение аргументов из стека
    mov edi, [ebp]  ; Указатель на результирующий массив res
    mov esi, [ebp + 4] ; Указатель на исходный массив a
    mov ebx, [ebp + 8] ; Индекс начала start
    mov ecx, [ebp + 12] ; Индекс конца end

    ; Копирование подмассива в res
    sub ecx, ebx        ; Размер подмассива
    inc ecx             ; Увеличиваем на 1, чтобы включить end
    mov edx, 0          ; Индекс в res
copy_loop:
    push ecx
    mov eax, [esi + ebx * 8] ; Загрузка элемента из a[start]
    mov ecx, [esi + ebx * 8 + 4]
    mov [edi + edx * 8], eax ; Сохранение в res
    mov [edi + edx * 8 + 4], ecx 
    pop ecx
    inc ebx                  ; Следующий индекс в a
    inc edx                  ; Следующий индекс в res
    loop copy_loop           ; Повторяем для всех элементов

    finit
    ; Сортировка пузырьком
    mov ecx, [ebp + 12]
    sub ecx, [ebp + 8]      ; Размер подмассива
    inc ecx                  ; Увеличиваем на 1
sort_outer_loop:
    push ecx                 ; Сохраняем счетчик внешнего цикла
    mov edx, 0               ; Индекс для внутреннего цикла
sort_inner_loop:
    fld qword ptr [edi + edx * 8] ; Загрузка числа с плавающей точкой в стек FPU
    fld qword ptr [edi + edx * 8 + 8] ; Загрузка следующего числа с плавающей точкой в стек FPU
    fcompp ; Сравнение двух чисел в стеке FPU и выталкивание их из стека
    fstsw ax ; Сохранение слова состояния FPU в AX
    sahf ; Загрузка AH в регистр флагов
    jbe no_swap ; Переход, если текущий элемент <= следующего
    
    fld qword ptr [edi + edx * 8] ; Обмен значениями
    fld qword ptr [edi + edx * 8 + 8]
    fstp qword ptr [edi + edx * 8]
    fstp qword ptr [edi + edx * 8 + 8]
no_swap:
    inc edx                  ; Следующий индекс
    loop sort_inner_loop     ; Повторяем для всех элементов
    pop ecx                  ; Восстанавливаем счетчик внешнего цикла
    loop sort_outer_loop     ; Повторяем для всех элементов

    ; Восстановление регистров
    pop edx
    pop ecx
    pop edi
    pop esi
    pop ebx

    ; Возврат указателя на res
    mov eax, [ebp]
    pop ebp
    ret                   ; Стек не очищаем
sort_cdecl endp

; double* sort(double* res, double* a, int start, int end).
sort_fastcall proc
    ; Сохранение регистров
    push ebp
    mov ebp, esp
    add ebp, 8
    push ebx
    push esi
    push edi
    push ecx
    push edx

    ; Извлечение аргументов из регистров и стека
    mov edi, ecx        ; Указатель на результирующий массив res
    mov esi, edx        ; Указатель на исходный массив а
    mov ebx, [ebp]  ; Индекс начала start
    mov ecx, [ebp + 4] ; Индекс конца end

    ; Копирование подмассива в res
    sub ecx, ebx        ; Размер подмассива
    inc ecx             ; Увеличиваем на 1, чтобы включить end
    mov edx, 0          ; Индекс в res
copy_loop:
    push ecx
    mov eax, [esi + ebx * 8] ; Загрузка элемента из a[start]
    mov ecx, [esi + ebx * 8 + 4]
    mov [edi + edx * 8], eax ; Сохранение в res
    mov [edi + edx * 8 + 4], ecx 
    pop ecx
    inc ebx                  ; Следующий индекс в a
    inc edx                  ; Следующий индекс в res
    loop copy_loop           ; Повторяем для всех элементов

    ; Сортировка пузырьком
    mov ecx, [ebp + 4]
    sub ecx, [ebp]           ; Размер подмассива
    inc ecx                  ; Увеличиваем на 1
    finit
sort_outer_loop:
    push ecx                 ; Сохраняем счетчик внешнего цикла
    mov edx, 0               ; Индекс для внутреннего цикла
sort_inner_loop:
    fld qword ptr [edi + edx * 8] ; Загрузка числа с плавающей точкой в стек FPU
    fld qword ptr [edi + edx * 8 + 8] ; Загрузка следующего числа с плавающей точкой в стек FPU
    fcompp ; Сравнение двух чисел в стеке FPU и выталкивание их из стека
    fstsw ax ; Сохранение слова состояния FPU в AX
    sahf ; Загрузка AH в регистр флагов
    jbe no_swap ; Переход, если текущий элемент <= следующего
    
    fld qword ptr [edi + edx * 8] ; Обмен значениями
    fld qword ptr [edi + edx * 8 + 8]
    fstp qword ptr [edi + edx * 8]
    fstp qword ptr [edi + edx * 8 + 8]
no_swap:
    inc edx                  ; Следующий индекс
    loop sort_inner_loop     ; Повторяем для всех элементов
    pop ecx                  ; Восстанавливаем счетчик внешнего цикла
    loop sort_outer_loop     ; Повторяем для всех элементов

    ; Восстановление регистров
    pop edx
    pop ecx
    pop edi
    pop esi
    pop ebx

    ; Возврат указателя на res
    mov eax, [ebp]
    pop ebp
    ret 8                  ; Очистка стека (2 аргумента * 4 байта)
sort_fastcall endp

; double* sort(double* res, double* a, int start, int end).
sort_cdecl_args proc c _res:DWORD, _a:DWORD, _start_:DWORD, _end_:DWORD
    ; Сохранение регистров
    push ebx
    push esi
    push edi
    push ecx
    push edx

    ; Извлечение аргументов из стека
    mov edi, _res        ; Указатель на результирующий массив res
    mov esi, _a          ; Указатель на исходный массив a
    mov ebx, _start_     ; Индекс начала start
    mov ecx, _end_       ; Индекс конца end

    ; Копирование подмассива в res
    sub ecx, ebx        ; Размер подмассива
    inc ecx             ; Увеличиваем на 1, чтобы включить end
    mov edx, 0          ; Индекс в res
copy_loop:
    push ecx
    mov eax, [esi + ebx * 8] ; Загрузка элемента из a[start]
    mov ecx, [esi + ebx * 8 + 4]
    mov [edi + edx * 8], eax ; Сохранение в res
    mov [edi + edx * 8 + 4], ecx 
    pop ecx
    inc ebx                  ; Следующий индекс в a
    inc edx                  ; Следующий индекс в res
    loop copy_loop           ; Повторяем для всех элементов

    finit
    ; Сортировка пузырьком
    mov ecx, _end_
    sub ecx, _start_          ; Размер подмассива
    inc ecx                  ; Увеличиваем на 1
sort_outer_loop:
    push ecx                 ; Сохраняем счетчик внешнего цикла
    mov edx, 0               ; Индекс для внутреннего цикла
sort_inner_loop:
    fld qword ptr [edi + edx * 8] ; Загрузка числа с плавающей точкой в стек FPU
    fld qword ptr [edi + edx * 8 + 8] ; Загрузка следующего числа с плавающей точкой в стек FPU
    fcompp ; Сравнение двух чисел в стеке FPU и выталкивание их из стека
    fstsw ax ; Сохранение слова состояния FPU в AX
    sahf ; Загрузка AH в регистр флагов
    jbe no_swap ; Переход, если текущий элемент <= следующего
    
    fld qword ptr [edi + edx * 8] ; Обмен значениями
    fld qword ptr [edi + edx * 8 + 8]
    fstp qword ptr [edi + edx * 8]
    fstp qword ptr [edi + edx * 8 + 8]
no_swap:
    inc edx                  ; Следующий индекс
    loop sort_inner_loop     ; Повторяем для всех элементов
    pop ecx                  ; Восстанавливаем счетчик внешнего цикла
    loop sort_outer_loop     ; Повторяем для всех элементов

    ; Восстанавливаем регистры
    pop edx
    pop ecx
    pop edi
    pop esi
    pop ebx

    ; Возврат указателя на res
    mov eax, _res
    ret
sort_cdecl_args endp

; double* sort(double* res, double* a, int start, int end).
sort_stdcall_args proc stdcall _res:DWORD, _a:DWORD, _start_:DWORD, _end_:DWORD
    ; Сохранение регистров
    push ebx
    push esi
    push edi
    push ecx
    push edx

    ; Извлечение аргументов из стека
    mov edi, _res        ; Указатель на результирующий массив res
    mov esi, _a          ; Указатель на исходный массив a
    mov ebx, _start_     ; Индекс начала start
    mov ecx, _end_       ; Индекс конца end

    ; Копирование подмассива в res
    sub ecx, ebx        ; Размер подмассива
    inc ecx             ; Увеличиваем на 1, чтобы включить end
    mov edx, 0          ; Индекс в res
copy_loop:
    push ecx
    mov eax, [esi + ebx * 8] ; Загрузка элемента из a[start]
    mov ecx, [esi + ebx * 8 + 4]
    mov [edi + edx * 8], eax ; Сохранение в res
    mov [edi + edx * 8 + 4], ecx 
    pop ecx
    inc ebx                  ; Следующий индекс в a
    inc edx                  ; Следующий индекс в res
    loop copy_loop           ; Повторяем для всех элементов

    finit
    ; Сортировка пузырьком
    mov ecx, _end_
    sub ecx, _start_          ; Размер подмассива
    inc ecx                  ; Увеличиваем на 1
sort_outer_loop:
    push ecx                 ; Сохраняем счетчик внешнего цикла
    mov edx, 0               ; Индекс для внутреннего цикла
sort_inner_loop:
    fld qword ptr [edi + edx * 8] ; Загрузка числа с плавающей точкой в стек FPU
    fld qword ptr [edi + edx * 8 + 8] ; Загрузка следующего числа с плавающей точкой в стек FPU
    fcompp ; Сравнение двух чисел в стеке FPU и выталкивание их из стека
    fstsw ax ; Сохранение слова состояния FPU в AX
    sahf ; Загрузка AH в регистр флагов
    jbe no_swap ; Переход, если текущий элемент <= следующего
    
    fld qword ptr [edi + edx * 8] ; Обмен значениями
    fld qword ptr [edi + edx * 8 + 8]
    fstp qword ptr [edi + edx * 8]
    fstp qword ptr [edi + edx * 8 + 8]
no_swap:
    inc edx                  ; Следующий индекс
    loop sort_inner_loop     ; Повторяем для всех элементов
    pop ecx                  ; Восстанавливаем счетчик внешнего цикла
    loop sort_outer_loop     ; Повторяем для всех элементов

    ; Восстанавливаем регистры
    pop edx
    pop ecx
    pop edi
    pop esi
    pop ebx

    ; Возврат указателя на res
    mov eax, _res
    ret
sort_stdcall_args endp

start:
    ; ; Вызов функции sort_stdcall
    ; push end_       ; Передача аргумента end
    ; push start_     ; Передача аргумента start
    ; lea eax, a      ; Загрузка адреса массива a
    ; push eax        ; Передача аргумента a
    ; lea eax, res    ; Загрузка адреса массива res
    ; push eax        ; Передача аргумента res
    ; call sort_stdcall

    ; ; Вызов функции sort_cdecl
    ; push end_       ; Передача аргумента end
    ; push start_     ; Передача аргумента start
    ; lea eax, a      ; Загрузка адреса массива a
    ; push eax        ; Передача аргумента a
    ; lea eax, res    ; Загрузка адреса массива res
    ; push eax        ; Передача аргумента res
    ; call sort_cdecl
    ; add esp, 16

    ; ; Вызов функции sort_fastcall
    ; lea ecx, res
    ; lea edx, a
    ; push end_       ; Передача аргумента end
    ; push start_     ; Передача аргумента start
    ; call sort_fastcall

    lea eax, res
    lea ebx, a
    invoke sort_cdecl_args, eax, ebx, start_, end_

    lea eax, res
    lea ebx, a
    invoke sort_stdcall_args, eax, ebx, start_, end_

    invoke ExitProcess, 0

end start