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
  str_format_num db 14 DUP("%02X "), 13, 10, 0

	; a db 15h, 0DDh, 34h, 4Bh, 57h, 7Fh, 0CDh, 05h, 2Dh, 13h, 44h, 0BBh, 12h, 3Fh
	; b db 15h, 0DDh, 34h, 4Bh, 57h, 7Fh, 0CDh, 05h, 2Dh, 13h, 44h, 0BBh, 12h, 3Fh

  a db 00h, 01h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
  b db 00h, 02h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h

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

    MOV EAX, 14
    for_start:            
        CMP EAX, 0        
        JNGE for_end      
      
        MOVZX ECX, r[EAX]
        push ECX

        DEC EAX          
        JMP for_start    
    for_end:
  
    push offset str_format_num
    call crt_printf

    push 0
    call ExitProcess
END START
