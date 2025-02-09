.686			
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
	a db 15h, 0DDh, 34h, 4Bh, 57h, 7Fh, 0CDh, 05h, 2Dh, 13h, 44h, 0BBh, 12h, 3Fh
	b db 15h, 0DDh, 34h, 4Bh, 57h, 7Fh, 0CDh, 05h, 2Dh, 13h, 44h, 0BBh, 12h, 3Fh

	r db 14 dup(?) ; Для результата резервируется 14 байт

.CODE
START:
    ; Вычитание младших 4 байт
    mov EAX, dword ptr a[0]
    sub EAX, dword ptr b[0]
    mov dword ptr r[0], EAX

    ; Вычитание следующих 4 байт с учетом переноса
    mov EAX, dword ptr a[4]
    sbb EAX, dword ptr b[4]
    mov dword ptr r[4], EAX

    ; Вычитание следующих 4 байт с учетом переноса
    mov EAX, dword ptr a[8]
    sbb EAX, dword ptr b[8]
    mov dword ptr r[8], EAX

    ; Вычитание оставшихся 2 байт с учетом переноса
    mov AX, word ptr a[12]
    sbb AX, word ptr b[12]
    mov word ptr r[12], AX

    MOV EAX, 18
  for_start_r:            ; метка, на которую проецируется цикл
    CMP EAX, 0         ; сравниваем с некоторым пределом
    JNGE for_end_r         ; условие - если счетчик больше или равен пределу, выход из цикла
    
  MOVSX ECX, r[EAX]
    push ECX
  MOV EBX, 18
  
    DEC EAX           ; действия цикла - увеличение счетчика
    JMP for_start_r       ; повторяем цикл
  for_end_r:
  
  push offset str_format_num
  call crt_printf
  add ESP, 5*20

    push 0
    call ExitProcess
END START
