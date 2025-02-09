EXTERN printf: PROC

.DATA
   fmt_int db "%d ", 0
.CODE

sum_numbers_fastcall PROC
    MOV RAX, RCX    ; ������ �������� ��������� � RCX
    ADD RAX, RDX    ; ������ �������� ��������� � RDX
    ADD RAX, R8     ; ������ �������� ��������� � R8
    ADD RAX, R9     ; �������� �������� ��������� � R9
    ADD RAX, [RSP + 5*8]    ; ��������� ��������� ��������� � �����, ������� � ������ RSP + 40
    ; 40 ���� - ��� ������� ��������� ��� 4 ��������� � ������ ��������
    RET
sum_numbers_fastcall ENDP 

sum_args_fastcall PROC
    PUSH RSI
    XOR RSI, RSI    ; RSI = 0
    XOR RAX, RAX    ; RAX = 0

    ; ECX - ����� ���������� ����������� �����
    CMP ECX, 0      ; ���� ECX <= 0, �� ������� RAX
    JLE exit_label        
    ADD RAX, RDX    ; RAX += RDX ; ���������� ������ ��������
    INC ESI         ; ESI++

    CMP ECX, 1      ; ���� ECX <= 1, �� ������� RAX
    JLE exit_label        
    ADD RAX, R8     ; RAX += R8 ; ���������� ������ ��������
    INC ESI         ; ESI++

    CMP ECX, 2      ; ���� ECX <= 2, �� ������� RAX
    JLE exit_label        
    ADD RAX, R9     ; RAX += R9  ; ���������� ������ ��������
    INC ESI         ; ESI++

    ; ��������� ���������� � ����� ��������
add_loop:
    CMP ESI, ECX    ; ���� ESI < ECX
    JGE exit_label
    ADD RAX, [RSP + RSI*8 + 24]
    INC ESI         ; ESI++
    JMP add_loop

exit_label:
    POP RSI
    RET

sum_args_fastcall ENDP


print_array PROC
    ; � RCX �������� ���������� ��������� � �������
    ; � RDX - ��������� �� ������
    PUSH RSI
    MOV RSI, RDX

print_loop:
    JECXZ exit_print ; ���� ECX=0, �� ����� ���������

    PUSH RCX     ; �������� RCX � �����, ������ ��� ������� printf �������� ���
    SUB RSP, 5*8 ; ��������� ������ ��� �������� ���������
    MOV RCX, offset fmt_int
    MOV RDX, [RSI]
    call printf ; printf("%d ", RDX)
    ADD RSP, 5*8
    POP RCX

    ADD RSI, 4 ; ������� � ���������� �������� � �������
    DEC ECX    ; ���������� ���������� ���������� ���������
    JMP print_loop

exit_print:
    POP RSI
    RET
print_array ENDP

END 