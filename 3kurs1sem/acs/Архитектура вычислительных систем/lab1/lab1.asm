 .386 ; Тип процессора
 .model flat, stdcall ; Модель памяти и стиль вызова подпрограмм
 option casemap: none ; Чувствительность к регистру
 ; --- Подключение файлов с кодом, макросами, константами, прототипами функций и т.д.
 include windows.inc
 include kernel32.inc
 include user32.inc
 include msvcrt.inc
 ; --- Подключаемые библиотеки ---
 includelib user32.lib
 includelib kernel32.lib
 includelib msvcrt.lib
 ; --- Сегмент данных ---
 .data 
	caption db "lab1", 0
	db "hello", 0
	str1 DB "MASM32", 0
	DB 250, 251, 252, 254
	a DW 500
	b DW 2
	c1 DW 250
	c2 DD 255*255*255*255, 100000
	c3 DD 0FFh, 0001FFFAh, 11223344h 
	float1 DD 13.5
	float2 DD 26.5
	double1 dq 11.01
	dmas DQ 5 DUP (5)
	message db "Hello World!", 0
	format 	db "str1", 	9, "%p", 9, "%p", 13, 10, 0
		db "a", 	9, "%p", 9, "%p", 13, 10
		db "b", 0
 ; --- Сегмент кода ---
 .code
 start:
	; printf("str1\t%p\t%p", str1, str1 + 6)
	push (offset a - 1) 	; str1 + 6 - адрес конца строки
 	push offset str1	; str1 - адрес начала строки
	push offset format	; "str1\t%p\t%p" - строка формата
	call crt_printf		; Вызов подпрограммы 
	; Очистка стека
	add esp, 3*4	; 3 аргумента, каждый занимает 4 байта 
	
	push 0			; Тип окна
	push offset caption	; Заголовок
	push offset message	; Сообщение
	push 0			; Дескриптор окна
	call MessageBoxA	
	
	push offset message
	call crt_puts
	add esp, 4
	call crt__getch
  	push NULL
  	call ExitProcess ; Выход из программы
 end start
