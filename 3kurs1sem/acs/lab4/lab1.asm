
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
	; x DB 6
	; y DW -2
	; z DW 2 ; res 1

	; x DB 6
	; y DW 4
	; z DW ? ; res 18

	x DB 12
	y DW -14
	z DW ? ; res 4

	format db "Result: %d", 0Ah, 0  
.CODE
START: 		
	; записываем х и у и расширяем
	xor ebx, ebx		; зануляем ebx		
	mov bl, x			; записываем беззнаковое число из x

	mov ax, y			; ax = y
	cwde				; расширение y до двойного слова 
  	xchg eax, ebx		; обмен местами содержимого EAX и EBX

	cmp eax, 10
	jb cond1			; беззнаковая проверка x < 10
	jmp func3			; иначе (x >= 10)
cond1:
	cmp ebx, 2
	jl func1			; знаковая проверка y < 2
	jmp func2			; иначе (x >=2)
func1:
	add eax, ebx		; складываем, получаем x + y
	push eax
	
	mov ax, z			; ax = z
	cwde				; расширение z до двойного слова 
	sub eax, ebx 		; вычитаем, получаем z - y

	xchg eax, ebx		; обмен местами содержимого EAX и EBX
	pop eax				
	cdq					; Расширение делимого со знаком ax -> dx:ax 
	idiv ebx			; делим, получаем (x + y)/(z - y)

	jmp func_end

func2:
	imul eax, ebx 		; умножаем, получаем x * y
	sub eax, 6			; умножаем, получаем x * y - 6

	jmp func_end

func3:
	add eax, ebx		; складываем, получаем x + y
	imul eax, eax		; умножаем, получаем (x+y)^2

	jmp func_end

func_end:
    ; Вывод результата в консоль
    push  eax                   
    push  offset format         
    call  crt_printf            

    ; Завершаем программу
    push 0
    call ExitProcess

END START
