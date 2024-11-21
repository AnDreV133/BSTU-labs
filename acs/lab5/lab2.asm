 .686 ;
 .model flat, stdcall ; 
 option casemap: none ; 

include c:\masm32\include\windows.inc
include c:\masm32\include\kernel32.inc
include c:\masm32\include\user32.inc
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\kernel32.lib
includelib c:\masm32\lib\msvcrt.lib
includelib c:\masm32\lib\user32.lib


.DATA
q dq 3.0
n dq 1.0
n_end dq 50.0
sum dq 0.0
part_degree_2_23 dq 2.0
part_degree_3_2 dq 3.0
part_degree_5_3 dq 5.0
degree dq 1.0
str_fmt db "n = %.1f, S = %.2f ", 13, 10, 0

.CODE

pow proc
    finit ;инициализация сопроц.
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
        FADD ST(0), ST(1)
        FMUL ST(0), ST(2)

        CMP EAX, 0
        je ret1

    FMUL ST(0), ST(4); умножение 2^(целая часть) * 2^(дробная часть)

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
    fld q
    fsqrt
    fst q
    j1:
        mov eax, dword ptr [n]
        mov edx, dword ptr [n + 4]
        mov dword ptr [degree + 4], edx
        mov dword ptr [degree], eax 
        call calculating_denominator
        fld1
        fdiv st(0), st(1)
        fld sum
        FADD
        fst sum

        finit
        fld part_degree_2_23
        fld n
        fmul st(0), st(1)
        fld part_degree_3_2
        fxch
        fdiv st(0), st(1)
        fst degree
        call calculating_denominator
        fld part_degree_3_2
        fdiv st(0), st(1)
        fld sum
        FADD
        fst sum

        finit
        fld part_degree_2_23
        fld n
        fmul st(0), st(1)
        fld part_degree_5_3
        fxch
        fdiv st(0), st(1)
        fst degree
        call calculating_denominator
        fld part_degree_5_3
        fdiv st(0), st(1)
        fld sum
        FADD
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