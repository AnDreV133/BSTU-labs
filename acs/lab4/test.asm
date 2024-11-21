.386			
.model flat, stdcall
option casemap: none

include windows.inc
include kernel32.inc
include msvcrt.inc
include user32.inc

includelib kernel32.lib
includelib msvcrt.lib
includelib user32.lib

.DATA
	
    h db -1
    x db 1,2,3,4
    y dw 4,3,2,1
    n dw 1

	format db "Result: %d", 0Ah, 0  

.CODE

START:

PUSH EAX ; z (4 байта)
PUSH DX  ; y (2 байта)
PUSH 100 ; x (4 байта)
CALL f

f proc
 PUSH EBX ; EBX сохранить в стеке
 PUSH EBP ; EBP сохранить в стеке
 MOV EBP, ESP ; В EBP скопировать ESP
 ADD EBP, 12 ; Сместить EBP ближе к аргументам
 MOV AX, [EBP+4] ; В AX поместить второй  аргумент y
 CWDE
 MOV EBX, [EBP] ; В EBX поместить первый аргумент x
 IMUL EBX
 ADD EAX, [EBP+6] ; К EAX прибавить третий аргумент z
 POP EBP ; Восстановить EBP
 POP EBX ; Восстановить EBX
 RET 10 ; Очистка стека от аргументов и возврат в вызывающую подпрограмму
f endp ; Результат – в EAX


END START
