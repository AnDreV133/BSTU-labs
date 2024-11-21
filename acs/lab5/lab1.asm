.686				
.model flat, stdcall
option casemap: none

; include windows.inc
include kernel32.inc
include msvcrt.inc
; include user32.inc

includelib	kernel32.lib
includelib	msvcrt.lib
; includelib user32.lib

.DATA
    x DQ 16.0
	y DQ 0.5

    format DB "res = %f"
.CODE
; x ^ y = 2 ^ (y * logx)
; 2 ^ t
; t = y log x
; дробная часть = b
; целая часть = a( с округлением до целого меньшего)
; b = t - a
; 2 ^ a * 2 ^ b

; результат записывается в edx:eax	
pow proc x_: real8, y_: real8
sub esp, 108	; для сохранения состояния сопроцессора 
fsave [esp]	; сохраняем состояние сопроцессора
; задаём значение управляещего регистра CWR
; такое, чтобы округление происходило
; в меньшую сторону
sub esp, 2	; память под значение регистра CWR xor eax, eax
fstcw [esp]	; сохраняем значение в память
mov ax, [esp]		
and ax, 0f0ffh	
or ax, 0600h 
mov [esp], ax	
fldcw [esp]	
add esp, 2
xor eax, eax
xor edx, edx

; начало вычислений 
fld y_

fldz 
db 0dfh, 0f0h+1	;FCOMIP y, 0 (сравнение y и 0)
jne y_none_zero	;y = 0 -> выход st0 = 1 
y_zero:

fld1 
jmp copy_top_to_result 

y_none_zero:

fld x_

; сравнение с нулём 
fldz
db 0dfh, 0f0h+1
jne	start_calculating

fldz
jmp copy_top_to_result 

start_calculating:
jb non_negative
inc edx

fabs 
non_negative:

; ST = x, y
fxch ST(1)
; st = y, x 
fld ST(0)
; st = |_ y _|, y, x 
frndint

db 0dfh, 0f0h+1
jne is_reminder_exist

no_reminder:
sub ESP, 4
fist dword ptr [ESP]
mov EAX, [ESP]
ADD ESP, 4
AND EAX, 1

is_reminder_exist:
FXCH ST(1)
fyl2x	; ST(0) = ylogx

fld ST(0)	; ST(0) = ylogx

frndint	; ST = a, t


FSUB ST(1), ST(0)	; ST(1) = b; st = a, b
fld1	; ST(0) = 1
fscale	; ST(0) = 2^a	; ST(0) = 2^a, ST(1) = a, ST(2) = t - a = b

fld ST(2)	; ST(0) = b, ST(1) = 2^a, ST(2) = a

f2xm1 
fld1
faddp ST(1), ST(0)
fmulp ST(1), ST(0)	; результат в вершине стека

cmp eax, 0
je copy_top_to_result

cmp edx, 0
je copy_top_to_result 

fchs 

copy_top_to_result:
 
sub esp, 8

fstp real8 ptr [esp] 
mov eax, [esp]
mov edx, [esp + 4]

add esp, 8

load_state: 
frstor [esp]
add esp, 108 
ret

pow endp

START:
    push dword ptr y[4]
    push dword ptr y[0]
    push dword ptr x[4]
    push dword ptr x[0]
    call pow

    ; Вывод результата в консоль
    push edx                
    push eax   
    push  offset format         
    call  crt_printf            

    ; Завершаем программу
    push 0
    call ExitProcess

END START











