.686
.model flat, stdcall
option casemap: none
include windows.inc
include kernel32.inc
include msvcrt.inc
includelib kernel32.lib
includelib msvcrt.lib

.data
  num_const_180 dd 180.0
  print_step db "sin(%.0f) = %.6f", 13, 10, 0
  print_fmt db "%.0f", 13, 10, 0
.code

start:
  finit ; Инициализация сопроцессора
  fldz ; ST(0) = n = 0

  fldpi ; ST(0) = PI  ST(1) = n
  fld num_const_180 ; ST(0) = 180  ST(1) = PI  ST(2) = n
  fdivp ST(1), ST(0) ; ST(0) = PI/180  ST(1) = n

  mov ecx, 361; while n < 50
loopStart:
  fld ST(1) ; ST(0) = n ST(1) = PI/180  ST(2) = n
  fmul ST(0), ST(1); ST(0) = n*PI/180 ST(1) = PI/180  ST(2) = n
  fsin ; ST(0) = sin(n*PI/180) ST(1) = PI/180  ST(2) = n

  sub esp, 16 ; Выделили папять
  fld ST(2) ; ST(0) = n
  fst qword ptr [esp] ; [esp] = ST(0)
  ffree ST(0)
  fincstp
  fst qword ptr [esp + 8] ; [esp+8] = ST(0)
  mov esi, ecx 
  push offset print_step 
  call crt_printf
  add esp, 20 ; Вернули в изначальное положение
  mov ecx, esi

  ffree ST(0)
  fincstp

  ; n = n + 1{
  fxch ; ST(0) = n
  fld1 ; ST(0) = 1 ST(1) = n
  faddp ST(1), ST(0) ; ST(0) = n + 1
  fxch ; ST(0) = S, ST(1) = n + 1
 
  dec ecx
  jne loopStart

  push 0 
  call ExitProcess 
end start
