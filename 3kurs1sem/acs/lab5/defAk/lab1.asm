.686				
.model flat, stdcall
option casemap: none

include windows.inc
include kernel32.inc
include msvcrt.inc
include user32.inc

includelib	kernel32.lib
includelib	msvcrt.lib
includelib user32.lib

.data
 a dd -1, 1, -2, 2, -3, 3, -4, 4, -5, 5
 n dd 10
 format db '%d ', 0
 new_line db 0Ah, 0

.code


; Подпрограмма удаления отрицательных чисел
delete proc
 push eax
 push ebx
 push edx
 push ebp
 mov ebp, esp
 push esi
 push ecx

 mov esi, [ebp+24]   ; Адрес массива a
 mov ecx, [ebp+20]   
 mov ecx, [ecx]      ; Размер массива n
 mov ebx, esi        ; Счетчик элементов
 mov edx, 0 

delete_loop:
 cmp ecx, 0
 je end_delete

 mov eax, [esi]
 cmp eax, 0
 jl skip
 mov [ebx], eax     ; Перемещаем положительный элемент
 add ebx, 4
 inc edx
 jmp next_iter

skip:
 jmp next_iter

next_iter:
 dec ecx
 add esi, 4
 jmp delete_loop

end_delete:
 mov eax, [ebp+20]
 mov [eax], edx   ; Обновляем размер массива n
 pop ecx
 pop esi
 pop ebp
 pop edx
 pop ebx
 pop eax
 ret 8        ; Освобождаем параметры стека

delete endp

; Подпрограмма вывода массива на экран
output proc
 push eax
 push ebp
 mov ebp, esp
 push esi
 push ecx

 mov esi, [ebp+16]   ; Адрес массива a
 mov ecx, [ebp+12]   ; Размер массива n

 ; Вызов crt_printf для вывода текста "Массив: "
;  push offset format
;  push offset msg
;  call crt_printf

;  mov ecx, [ebp+8]
output_loop:
 cmp ecx, 0
 je end_output

 mov eax, [esi]
 push ecx
 push eax       ; Загружаем значение на стек
 push offset format  ; Загружаем формат на стек
 call crt_printf   ; Вызываем crt_printf
 
 add esp, 8
 pop ecx
 add esi, 4
 dec ecx
 jmp output_loop

end_output:
 push offset new_line
 call crt_printf
 add esp, 4
 pop ecx
 pop esi
 pop ebp
 pop eax
 ret 8        ; Освобождаем параметры стека

output endp

start:
 ; Выводим исходный массив в консоль
 push offset a
 push n
 call output

 ; Удаляем отрицательные числа из массива
 push offset a
 push offset n
 call delete

 ; Выводим результирующий массив в консоль
 push offset a
 push n
 call output
 
 push 0
 call ExitProcess

end start