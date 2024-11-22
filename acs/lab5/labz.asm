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

.data
  format db "sin(%d) = %f", 0dh, 0ah, 0
  const_pi dd 314159265
  const_180 dd 1800000000
  angle_degrees dw 0

.code
start:
  mov ecx, 361 ; Цикл от 0 до 360
  mov word ptr angle_degrees, 0

loop_start:
  cmp ecx, 1
  je loop_end

  ; Преобразование градусов в радианы
  fild word ptr angle_degrees
  fimul const_pi; приблизительно Pi * 10^8
  fidiv const_180 ; приблизительно 180 * 10^8

  ; Вычисление синуса
  fsin

  ; Вывод результата
  push eax
  push dword ptr angle_degrees
  fstp qword ptr [esp+4] ; Сохранение результата синуса в стек
  push offset format
  call crt_printf
  add esp, 16 ; очистка стека
  pop eax

  ; Увеличение угла
  inc word ptr angle_degrees
  loop loop_start


loop_end:
  invoke ExitProcess, 0





end start
