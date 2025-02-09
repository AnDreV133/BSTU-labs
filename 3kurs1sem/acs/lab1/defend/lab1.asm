 .386 ; ��� ����������
 .model flat, stdcall ; ������ ������ � ����� ������ �����������
 option casemap: none ; ���������������� � ��������
 ; --- ����������� ������ � �����, ���������, �����������, ����������� ������� � �.�.
 include windows.inc
 include kernel32.inc
 include user32.inc
 include msvcrt.inc
 ; --- ������������ ���������� ---
 includelib user32.lib
 includelib kernel32.lib
 includelib msvcrt.lib
 ; --- ������� ������ ---
 .data
strt DB "Some string", 0
DW 400
a DF 900
mas DD 4*8 DUP (3)
s DQ 15.7

str_fmt db "%s", 9, "%p", 9, "%p", 9, "%d", 13, 10, "%s", 9, "%p", 9, "%p", 9, "%d", 13, 10, "%s", 9, "%p", 9, "%p", 9, "%d", 13,10,"%s", 9, "%p", 9, "%p", 9, "%d", 13,10,"%s", 9, "%p", 9, "%p", 9, "%d", 13, 10, 0
strt_name db "strt", 0 		
_name db "-", 0		
a_name db "a", 0
mas_name db "mas", 0
s_name db "s", 0 

out_buffer DB 128 dup(0) 
caption DB "Variables introduced:", 0

.code
start:

; 5 row
push 8
push offset s+7
push offset s
push offset s_name

; 4 row
push (offset s) - (offset mas) 
push offset s-1
push offset mas
push offset mas_name

; 3 row
push (offset mas) - (offset a) 
push offset mas-1
push offset a
push offset a_name

; 2 row
push 2
push offset a-1
push offset a-1-2
push offset _name

; 1 row
push (offset a-1-2) - (offset strt) 
push offset a-1-2-1
push offset strt
push offset strt_name

push offset str_fmt		; Адрес строки формата

push offset out_buffer 
call crt_sprintf
; add esp, 4*13

; push offset out_buffer 	
call crt_puts
; add esp, 4*1

push 0			; Тип окна
push offset caption	; Заголовок окна
push offset out_buffer	; Сообщение
push 0			; Дескриптор окна
call MessageBoxA	

; add esp, 5*9			; Очистка стека от аргументов. Второй операнд - количество аргументов, умноженное на 4	

end start