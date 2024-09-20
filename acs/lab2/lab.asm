.686				
 .model flat, stdcall
 option casemap: none

;  include windows.inc
;  include kernel32.inc
;  include msvcrt.inc
;  includelib	kernel32.lib
;  includelib	msvcrt.lib


.CODE
START:
	MOV EDX, [ESI+7]
	ADD BL, AL
	MOV AX, 1001b
	ADD AX, [EBX] 
	CMP DI, AX 
	SUB EAX, [ECX*4+EAX+ 'Z']
	AND WORD PTR [EDI], 12345h

END START
