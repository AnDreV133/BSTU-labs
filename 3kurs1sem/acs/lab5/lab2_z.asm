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
    x DQ 2.0
	y DQ 3.0

    currentValues DD 100 dup(?)

    q DQ 0.3

    cur_n DD 30

    format1 DB "n = %d "
    format2 DB "res = %f", 0Ah
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
fxch ST(1)
fyl2x	; ST(0) = ylogx

fld ST(0)	; ST(0) = ylogx

frndint	; ST = a, t


fsub ST(1), ST(0)	; ST(1) = b; st = a, b
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


; ////////////////////////////////////


move_to_esp_real8	macro	val_ 
sub esp, TYPE real8

fld	real8 ptr val_ 
fstp real8 ptr [esp]
endm

move_top_to_esp	macro 
sub esp, TYPE real8

fstp real8 ptr [esp] 
endm

copy_result_to macro p_ 
mov [p_], eax
mov [p_ + 4], edx 
endm

; ///////////////////////////////




start:
  finit ; Инициализация сопроцессора
  fld1 ; S(0) = n = 1
  fldz ; S(0) = S = 0; S(1) = 1
  
  mov ecx, 50 ; while n < 50
loopStart:
; Вывод "S = %f, n = %f\n"{
    sub esp, 16 ; Выделили папять
    fst qword ptr [esp] ; [esp] = ST(0)
    fxch ; ST(1) = ST(0) ST(0) = ST(1)
    fst qword ptr [esp + 8] ; [esp+8] = ST(0)
    fxch ; ST(1) = ST(0) ST(0) = ST(1)
    mov esi, ecx 
    push offset print_step 
    call crt_printf
    add esp, 20 ; Вернули в изначальное положение
    mov ecx, esi
;}
; 3/(q^n - 1){
    fld1 ; ST(0) = 1 ST(1) = S ST(2) = n
    sub esp, 8 ; Вделили место
    fld ST(2) ; ST(0) = n ST(1) = 1 ST(2) = S ST(3) = n
    fstp dword ptr [esp + 4] ; [esp + 4] = n 
    mov eax, q
    mov dword ptr [esp], eax ; [esp] = q
    call pow ; ST(0) = q ^ n, ST(1) = 1 ST(2) = S ST(3) = n
    fsubrp ST(1), ST(0) ; ST(0) = q ^ n - 1, ST(1) = S ST(2) = n
    fld num_const_3 ; ST(0) = 3 ST(1) = q ^ n - 1, ST(2) = S ST(3) = n
    fdivrp ST(1), ST(0); ST(0) = 3/(q ^ n - 1), ST(1) = S ST(2) = n
;}

; 4/(q^2n - 1){
    fld1 ; ST(0) = 1
    sub esp, 8 ; Вделили место
    fld ST(3) ; ST(0) = n ST(1) = 1
    fld num_const_2; ; ST(0) = 2 ST(0) = n ST(1) = 1
    fmulp ST(1), ST(0); ST(0) = n*2 ST(1) = 1
    fstp dword ptr [esp + 4] ; [esp + 4] = n*2
    mov eax, q
    mov dword ptr [esp], eax ; [esp] = q
    call pow ; ST(0) = q ^ 2n, ST(1) = 1 
    fsubrp ST(1), ST(0) ; ST(0) = q ^ 2n - 1
    fld num_const_4 ; ST(0) = 4 ST(1) = q ^ 2n - 1
    fdivrp ST(1), ST(0); ST(0) = 4/(q^2n - 1) ST(1) = 3/(q^n - 1) ST(2) = S ST(3) = n
;}
    fchs ; ST(0) = -4/(q^2n - 1)
    faddp ST(1), ST(0) ;ST(0) = -4/(q^2n - 1) + 3/(q^n - 1) ST(1) = S ST(2) = n

; 1/(q^4n - 1){
    fld1 ; ST(0) = 1
    sub esp, 8 ; Вделили место
    fld ST(3) ; ST(0) = n ST(1) = 1
    fld num_const_4; ; ST(0) = 4 ST(0) = n ST(1) = 1
    fmulp ST(1), ST(0); ST(0) = n*4 ST(1) = 1
    fstp dword ptr [esp + 4] ; [esp + 4] = n*4
    mov eax, q
    mov dword ptr [esp], eax ; [esp] = q
    call pow ; ST(0) = q ^ 4n, ST(1) = 1 
    fsubrp ST(1), ST(0) ; ST(0) = q ^ 4n - 1
    fld1 ; ST(0) = 1 ST(1) = q ^ 4n - 1
    fdivrp ST(1), ST(0); ST(0) = 1/(q^4n - 1) ST(1) = -4/(q^2n - 1) + 3/(q^n - 1) ST(2) = S ST(3) = n
;}
    faddp ST(1), ST(0) ;ST(0) = 1/(q^4n - 1) -4/(q^2n - 1) + 3/(q^n - 1) ST(1) = S ST(2) = n

; 1/n{
  fld1 ; ST(0) = 1 
  fld ST(3) ; ST(0) = n ST(1) = 1
  fdivp ST(1), ST(0) ; ST(0) = 1/n ST(1) = 3/(q^n - 1) - 4/(q^2n - 1) 1/(q^4n - 1) ST(2) = S ST(3) = n
;}
  fmulp ST(1), ST(0); ST(0) = 1/n * (3/(q^n - 1) - 4/(q^2n - 1) 1/(q^4n - 1)) 

  faddp ST(1), ST(0); ST(0) = S + 1/n * (3/(q^n - 1) - 4/(q^2n - 1) 1/(q^4n - 1)) ST(1) = n

; n = n + 1{
    fxch ; ST(0) = n
    fld1 ; ST(0) = 1 ST(1) = n
    faddp ST(1), ST(0) ; ST(0) = n + 1
    fxch ; ST(0) = S, ST(1) = n + 1
  ;}
  dec ecx
  jne loopStart

  fxch
  ffree ST(0)
  fincstp
  sub esp, 8
  fstp qword ptr [esp]
  push offset print_fmt
  call crt_printf
  add esp, 8
  call crt__getch ; Задержка ввода, getch()
  ; Вызов функции ExitProcess(0)
  push 0 ; Поместить аргумент функции в стек
  call ExitProcess ; Выход из программы
end start


; /////////////////////////////////////////



calculate_sum_of_series proc uses ecx n_ : dword 
LOCAL sum_ : real8
LOCAL current_grade_q_ : real8

sub esp, 108	; для сохранения состояния сопроцессора
fsave [esp]	; сохраняем состояние сопроцессора

fldz
fst	current_grade_q_	; обнуляем локальные переменные 
fstp sum_

fld1	; st = n1

mov ecx, n_ 

calculate_sum:
fld st(0) 
fld st(0)
; st = n, n
; посчитаем q ** n

; fst real8 ptr currentValues[0]
 
move_top_to_esp 
move_to_esp_real8 q
call pow	; результат -> edx:eax

; copy_result_to current_grade_q_ 
mov dword ptr[current_grade_q_], eax
mov dword ptr[current_grade_q_ + 4], edx

fld current_grade_q_	; st = q**n, n

fld st(0)	; st = q**n, q**n, n 
fst real8 ptr currentValues[1 * TYPE real8]

fdivr st(0), st(2)	; st = n/q**n , q**n, n

fadd sum_	; st = sum + cur_sum
fstp sum_	; sum = cur_sum
; st = q ** n, n
fmul st(0), st(0)	; q**2n

fxch st(1)	; n, q**2n
fld1
fadd st(0), st(0)	; st = 2, n, q**2n
fmulp st(1), st(0)	; st = 2n, q**2n
; fst real8 ptr currentValues[3 * TYPE real8] 
fxch st(1)	; q**2n, 2n
fst real8 ptr currentValues[2 * TYPE real8]

fld st(0)	; q**2n, q**2n, 2n
fdivr st(0), st(2)	; st = 2n/q**2n , q**2n, 2n
; fst real8 ptr currentValues[5 * TYPE real8]
fadd sum_	; st = sum + cur_sum
fstp sum_	; sum = cur_sum

; st = q ** n, n
fmul st(0), st(0)	; q**4n
fst real8 ptr currentValues[3 * TYPE real8] 
fxch st(1)	; n, q**4n
fld1
 
fadd st(0), st(0)	; st = 2, n, q**4n
fmulp st(1), st(0)	; st = 4n, q**4n
; fst real8 ptr currentValues[6 * TYPE real8]

fxch st(1)	; q**4n, 4n
fld st(0)	; q**4n, q**4n, 4n
fdivr st(0), st(2)	; st = n/q**n , q**n, n
 
; fst real8 ptr currentValues[8 * TYPE real8]
fadd sum_	; st = sum + cur_sum, q**n, n
fstp sum_	; st = q**n, n
fstp st(0)	; n
fstp st(0) 
fld1
faddp st(1), st(0) 
dec ecx
cmp ecx, 0
jne calculate_sum

load_state:
frstor [esp]	; загружаем состояние сопроцессора 
add esp, 108

mov eax, dword ptr [sum_]
mov edx, dword ptr [sum_ + 4]

fst real8 ptr currentValues[0] 
fld sum_

ret
calculate_sum_of_series endp











START:

mov ecx, 1
print_sums_for:
cmp ecx, 50
jg print_sums_for_end

; push ecx
; push offset format1
; call crt_printf
; push 0

mov cur_n, ecx
invoke calculate_sum_of_series, cur_n

mov ebx, offset currentValues 
mov dword ptr [ebx], eax
mov dword ptr [ebx + 4], edx

sub esp, sizeof currentValues 

mov esi, offset currentValues
mov edi, esp 

mov ecx, 4 

start_loop:
mov eax, dword ptr [esi] 
mov [edi], eax

mov eax, dword ptr [esi + 4] 
mov [edi + 4], eax

add esi, TYPE real8 
add edi, TYPE real8
loop start_loop

invoke crt_printf, offset format2, cur_n 
add esp, sizeof currentValues

mov ecx, cur_n 
inc ecx
jmp print_sums_for 

print_sums_for_end:
invoke ExitProcess, 0

END START



