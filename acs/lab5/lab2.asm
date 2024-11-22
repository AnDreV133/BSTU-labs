 .686 ;
 .model flat, stdcall ; 
 option casemap: none ; 

include windows.inc
include kernel32.inc
include user32.inc
include msvcrt.inc
includelib kernel32.lib
includelib msvcrt.lib
includelib user32.lib


.DATA
q dq 3.0
n dq 1.0
buf dq ?
n_end dq 25.0
sum dq 0.0
const_2 dq 2.0
const_4 dq 4.0
; part_degree_5_3 dq 5.0
degree dq 1.0
str_fmt db "n = %.1f, S = %.2f ", 13, 10, 0

.CODE

pow proc
    fld qword PTR [ESP + 4] ;st: x
    fldz ;st: 0 x
    db 0DBh , 0F0h+1 ;FCOMI 0, x (сравнение х и 0)
    je ret1 ;x = 0 -> выход, st0 = 0

    fld qword PTR [ESP + 12] ;st: y 0 x
    db 0DBh , 0F0h+1 ;FCOMI y, 0 (сравнение y и 0)
    fld1 ;st: 1 y 0 x
    je ret1 ;y = 0 -> выход st0 = 1

    finit ;отчистка+инициализация
    fld qword PTR [ESP + 4] ;st: x -2
    fld ST(0) ;st: x x -2 -2
    fabs ;st: abs(x) x 2 -2
    fdiv ST(0), ST(1) ;st: sign(x) x -1 -2

    fxch ST(1) ;st: x, sign(x) -2 -1
    fld qword PTR [ESP + 12] ;st: y, x, sign(x) 2 -2 -1
    fld ST(0) ;st: y, y, x, sign(x) 2 -2 -1
    frndint ; округляем y

    db 0dfh, 0f0h+1; сравнение y округленного и не округленного
    jne raise_to_degree

    save_sign_x:
        fist dword ptr [ESP - 4] ; [esp - 4] = целая часть st(0)
        mov EAX, [ESP - 4]
        AND EAX, 1

    raise_to_degree:
        FXCH ST(1) ;st: x, y, sign(x) 2 2 -1
        FABS ;st: abs(x), y, sign(x) 2 2 -1
        FYL2X ;st: t, sign(x) 2 -1
        FLD1 ;st: 1, t, sign(x) 1 2 -1 
        FSCALE ; 2^[t], t, sign(x) 4 2 -1
        FLD1
        FLD ST(2)
        FPREM
        F2XM1
        FADDp ST(1), ST(0)
        FMULp ST(1), ST(0)

        CMP EAX, 0
        je ret1

        FMUL ST(0), ST(2); умножение 2^(целая часть) * 2^(дробная часть)
    ret1:
        RET 16
pow endp

calculating_denominator proc
    finit
    xor eax, eax
    xor edx, edx
    mov eax, dword ptr [degree]
    mov edx, dword ptr [degree + 4]

    push edx
    push eax

    xor eax, eax
    xor edx, edx
    mov eax, dword ptr [q]
    mov edx, dword ptr [q + 4]

    push edx
    push eax

    call pow

    ret 0

calculating_denominator endp

START:
    finit
    fld q
    fsqrt
    fstp q
    j1:
        mov eax, dword ptr [n]
        mov edx, dword ptr [n + 4]
        mov dword ptr [degree + 4], edx
        mov dword ptr [degree], eax 
        call calculating_denominator
        fld n
        fdiv st(0), st(1)
        fld sum
        fadd
        fst sum

        finit
        fld const_2
        fld n
        fmulp st(1), st(0)
        fst degree
        call calculating_denominator
        fld degree
        fdiv st(0), st(1)
        fld sum
        fadd
        fst sum

        finit
        fld const_4
        fld n
        fmulp st(1), st(0)
        fst degree
        call calculating_denominator
        fld degree
        fdiv st(0), st(1)
        fld sum
        fadd
        fst sum

        sub ESP, 8 ; Выделение памяти в стеке
        fstp qword ptr [ESP] ; Вытолкнуть число типа float на вершину стека
        mov eax, dword ptr [n]
        mov edx, dword ptr[n + 4]
        push edx
        push eax
        push offset str_fmt
        call crt_printf
        add esp, 4*5

        fld n_end
        fld n
        db 0DBh , 0F0h+1
        je ret2
        fld1
        fadd st(0), st(1)
        fst n
        jmp j1

    ret2:
        push 0
        call ExitProcess
END START