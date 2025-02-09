.686				; Тип процессора
 .model flat, stdcall	; Модель памяти и стиль вызова подпрограмм
 option casemap: none	; Чувствительность к регистру

  ; --- Подключение файлов с кодом, макросами, константами, прототипами  функций и т.д.
 include windows.inc
 include kernel32.inc
 include msvcrt.inc
 includelib	kernel32.lib
 includelib	msvcrt.lib
	
  ; --- Сегмент данных ---
 .data
	a db 127	; Первое число
	b db 128	; Второе число
	sum db 8 dup(?)	; Буфер для хранения строки с суммой чисел a и b
	message db "It is assembler sum of unsigned char numbers", 0

  ; --- Сегмент кода ---
 .code
 start:
	MOV AX, 0		; AX = 0
	MOV AL, a		; AL = a
	MOV BL, b		; BL = b
	ADD AL, BL		; AL = AL + BL
	MOV ECX, 10		; ECX = 10
	
	DIV CL		; AL = AX / 10, AH = AX % 10
	MOV sum[2], AH	; sum[2] = a % 10
	
	MOV AH, 0
	DIV CL		; AL = AX / 10, AH = AX % 10
	MOV sum[1], AH	; sum[1] = a / 10 % 10

	MOV sum[0], AL	; sum[0] = a / 100
	
	; Нужно добавить код цифры '0', чтобы преобразовать цифры от 0 до 9 в символы от '0' до '9' 
	ADD sum[0], '0'	; sum[0] += '0'
	ADD sum[1], '0'	; sum[1] += '0'
	ADD sum[2], '0'	; sum[2] += '0'
	MOV sum[3], 0	; sum[3] = 0	; Признак конца строки - нулевой символ
	
	push offset sum
	call crt_puts	; puts(sum)
	ADD ESP, 4		; Очистка стека от аргумента
	
	push offset message
	call crt_puts	; puts(message)
	ADD ESP, 4		; Очистка стека от аргумента

	call crt__getch ; Задержка ввода, getch()
	; Вызов функции ExitProcess(0)
	push 0	; Поместить аргумент функции в стек
	call ExitProcess ; Выход из программы
 end start
