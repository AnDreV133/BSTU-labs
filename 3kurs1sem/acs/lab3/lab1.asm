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

.DATA
	; a DW 5
	; b DW 4
	; d DB 2 ; res 34

	; a DW -5
	; b DW 4
	; d DB 2 ; res -21

	; a DW 5
	; b DW 4
	; d DB -2 ; res 4

	a DW 6
	b DW 4
	d DB 3 ; res -21

	format db "Result: %d", 0Ah, 0  
.CODE
START: 							; ab+ad+bd-(a+1)/d-1
	; Вычисляем a * b
    movsx eax, a   				; Загрузить a в EAX и расширить до 32 бит
    movsx ebx, b    			; Загрузить b в EBX и расширить до 32 бит
    imul  eax, ebx              ; Выполнить a * b

	push  eax 					; Сохраняем результат в стеке

	; Вычисляем a * d
    movsx eax, a     			; Загрузить a в EAX и расширить до 32 бит
    movsx ebx, d    			; Загрузить d в EBX и расширить до 32 бит
    imul  eax, ebx				; Выполнить a * d
    
	; Вычисляем a * b + a * d
	add   dword ptr [esp], eax  ; Добавить результат сложения в стек

	; Вычисляем b * d
    movsx eax, b    			; Загрузить b в EAX и расширить до 32 бит
    movsx ebx, d    			; Загрузить d в EBX и расширить до 32 бит
    imul  eax, ebx              ; Выполнить b * d
    
	; Вычисляем a * b + a * d + b * d
	add   dword ptr [esp], eax  ; Добавить результат сложения в стек

    ; Вычислим a + 1
	movsx eax, a			    ; Загрузить a в EAX и расширить до 32 бит
	inc   eax					; Прибавляем 1

	; Вычислим (a + 1) / d
    movsx ebx, d    			; Загрузить d в EBX и расширить до 32 би
	cdq 						; Расширение делимого со знаком ax -> dx:ax 
	idiv  ebx					; Делим предыдущий результат на d

	; Вычислим a * b + a * d + b * d - (a + 1) / d
	sub   dword ptr [esp], eax 	; Вычитаем из суммы, лежащей в стеке 

	; Вычислим a * b + a * d + b * d - (a + 1) / d - 1
	dec   dword ptr [esp]		; Вычитаем 1

	; Восстановление результата из стека в EAX
    pop   eax                   ; Переносим из стека в EAX
	
    ; Вывод результата в консоль
    push  eax                   ; Помещаем результат в стек для вывода
    push  offset format         ; Помещаем строку формата в стек
    call  crt_printf            ; Вызов printf для вывода результата

    ; Завершаем программу
    push 0
    call ExitProcess

END START
