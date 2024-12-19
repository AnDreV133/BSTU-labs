.386
.model flat,stdcall
option casemap:none
include C:\masm32\include\windows.inc
include C:\masm32\include\user32.inc
include C:\masm32\include\kernel32.inc
include C:\masm32\include\gdi32.inc
include C:\masm32\include\msvcrt.inc

includelib C:\masm32\lib\msvcrt.lib
includelib C:\masm32\lib\user32.lib 
includelib C:\masm32\lib\kernel32.lib 
includelib C:\masm32\lib\gdi32.lib

WinMain proto :DWORD,:DWORD,:DWORD,:DWORD

.data
ClassName db "SimpleWinClass",0
AppName  db "Triangle Example",0

.data?
hInstance HINSTANCE ?
hPen     HPEN ? ; Дескриптор для пера

.code
start:
    invoke GetModuleHandle, NULL
    mov hInstance, eax
    invoke WinMain, hInstance, NULL, NULL, SW_SHOWDEFAULT
    invoke ExitProcess, eax

WinMain proc hInst:HINSTANCE, hPrevInst:HINSTANCE, CmdLine:LPSTR, CmdShow:DWORD
    LOCAL wc:WNDCLASSEX
    LOCAL msg:MSG
    LOCAL hwnd:HWND

    mov wc.cbSize, SIZEOF WNDCLASSEX
    mov wc.style, CS_HREDRAW or CS_VREDRAW
    mov wc.lpfnWndProc, OFFSET WndProc
    mov wc.cbClsExtra, NULL
    mov wc.cbWndExtra, NULL
    push hInst
    pop wc.hInstance
    mov wc.hbrBackground, COLOR_WINDOW+1
    mov wc.lpszMenuName, NULL
    mov wc.lpszClassName, OFFSET ClassName
    invoke LoadIcon, NULL, IDI_APPLICATION
    mov wc.hIcon, eax
    mov wc.hIconSm, eax
    invoke LoadCursor, NULL, IDC_ARROW
    mov wc.hCursor, eax
    invoke RegisterClassEx, addr wc

    invoke CreateWindowEx, NULL, ADDR ClassName, ADDR AppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 250, 300, NULL, NULL, hInst, NULL
    mov hwnd, eax
    invoke ShowWindow, hwnd, CmdShow
    invoke UpdateWindow, hwnd

    .WHILE TRUE
        invoke GetMessage, ADDR msg, NULL, 0, 0
        .BREAK .IF (!eax)
        invoke TranslateMessage, ADDR msg
        invoke DispatchMessage, ADDR msg
    .ENDW
    mov eax, msg.wParam
    ret
WinMain endp

WndProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    LOCAL hdc:HDC
    LOCAL ps:PAINTSTRUCT
    LOCAL hOldPen:HPEN     ; Для хранения старого пера
    LOCAL hBluePen:HPEN    ; Для синего пера
    LOCAL hRedPen:HPEN
    LOCAL hWhitePen:HPEN
    LOCAL pointsOfShape[6]:POINT    ; Точки пятиугольника
    LOCAL pointsOfLines[3]:POINT   ; Точки линий
    LOCAL pointsOfEyelet[10]:POINT  ; Точки петелек 

    .IF uMsg == WM_DESTROY
        invoke PostQuitMessage, 0
    .ELSEIF uMsg == WM_PAINT
        invoke BeginPaint, hWnd, ADDR ps
        mov hdc, eax

        ; Определяем точки пятиугольника
        mov pointsOfShape[0 * TYPE POINT].x, 110
        mov pointsOfShape[0 * TYPE POINT].y, 80

        mov pointsOfShape[1 * TYPE POINT].x, 150
        mov pointsOfShape[1 * TYPE POINT].y, 110

        mov pointsOfShape[2 * TYPE POINT].x, 135
        mov pointsOfShape[2 * TYPE POINT].y, 155

        mov pointsOfShape[3 * TYPE POINT].x, 85
        mov pointsOfShape[3 * TYPE POINT].y, 155

        mov pointsOfShape[4 * TYPE POINT].x, 70
        mov pointsOfShape[4 * TYPE POINT].y, 110

        mov pointsOfShape[5 * TYPE POINT].x, 110
        mov pointsOfShape[5 * TYPE POINT].y, 80

        ; Красный цвет
        invoke CreatePen, PS_SOLID, 10, 0FFh 
        mov hRedPen, eax

        ; Выбираем красное перо в контекст устройства
        invoke SelectObject, hdc, hRedPen
        mov hOldPen, eax ; Сохраняем старое перо

        ; Рисуем пятиугольник без заполнения
        invoke Polyline, hdc, ADDR pointsOfShape, 6

        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        ; Создаем синее перо
        invoke CreatePen, PS_SOLID, 10, 0FF0000h 
        mov hBluePen, eax

        ; Выбираем синее перо в контекст устройства
        invoke SelectObject, hdc, hBluePen
        mov hOldPen, eax

        ; 1
        mov pointsOfLines[0 * TYPE POINT].x, 130
        mov pointsOfLines[0 * TYPE POINT].y, 64

        mov pointsOfLines[1 * TYPE POINT].x, 195
        mov pointsOfLines[1 * TYPE POINT].y, 113

        mov pointsOfLines[2 * TYPE POINT].x, 163
        mov pointsOfLines[2 * TYPE POINT].y, 216

        invoke Polyline, hdc, ADDR pointsOfLines, 3

        ; 2
        mov pointsOfLines[0 * TYPE POINT].x, 173
        mov pointsOfLines[0 * TYPE POINT].y, 120

        mov pointsOfLines[1 * TYPE POINT].x, 145
        mov pointsOfLines[1 * TYPE POINT].y, 197

        mov pointsOfLines[2 * TYPE POINT].x, 37
        mov pointsOfLines[2 * TYPE POINT].y, 197

        invoke Polyline, hdc, ADDR pointsOfLines, 3
        
        ; 3
        mov pointsOfLines[0 * TYPE POINT].x, 132
        mov pointsOfLines[0 * TYPE POINT].y, 176

        mov pointsOfLines[1 * TYPE POINT].x, 50
        mov pointsOfLines[1 * TYPE POINT].y, 176

        mov pointsOfLines[2 * TYPE POINT].x, 13
        mov pointsOfLines[2 * TYPE POINT].y, 74

        invoke Polyline, hdc, ADDR pointsOfLines, 3

        ; 4
        mov pointsOfLines[0 * TYPE POINT].x, 132
        mov pointsOfLines[0 * TYPE POINT].y, 176

        mov pointsOfLines[1 * TYPE POINT].x, 50
        mov pointsOfLines[1 * TYPE POINT].y, 176

        mov pointsOfLines[2 * TYPE POINT].x, 13
        mov pointsOfLines[2 * TYPE POINT].y, 74

        invoke Polyline, hdc, ADDR pointsOfLines, 3

        ; Восстанавливаем старое перо
        invoke SelectObject, hdc, hOldPen

        ; Удаляем красное перо
        invoke DeleteObject, hRedPen

        ; Удаляем синее перо
        invoke DeleteObject, hBluePen

        invoke EndPaint, hWnd, ADDR ps
    .ELSE
        invoke DefWindowProc, hWnd, uMsg, wParam, lParam
        ret
    .ENDIF
    xor eax, eax
    ret
WndProc endp

end start