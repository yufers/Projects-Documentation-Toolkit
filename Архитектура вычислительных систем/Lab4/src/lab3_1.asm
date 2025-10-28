.386
 .model flat, stdcall
 option casemap: none
 include c:\masm32\include\kernel32.inc
 include c:\masm32\include\msvcrt.inc
 includelib c:\masm32\lib\kernel32.lib
 includelib c:\masm32\lib\msvcrt.lib

.data
	x db -10
	y dd -40
	z dw 10
	format db "a = %d", 0
.code
start:
	MOV ECX, y  		; Расширение байта x до двойного слова в EAX
	MOV AL, x 		; В AL поместить x
	CBW 			; Расширение x (EAX) до 2 байт
	CWDE 			; Расширение x (EAX) до 4 байт
	ADD ECX, EAX
	MOVSX EDX, z
	ADD ECX, EDX

	CMP ECX, 10 		; Если AL > 10, то переход на j2
	JG j2

	CMP AL, 0
	JG j1 			; Если AL > 0, то переход на j1

	MOV EAX, y
	MOVSX ECX, z
	IMUL ECX		; EDX:EAX = y * z
	MOV EBX, 2
	IDIV EBX
	MOVSX EDX, x
	ADD EAX, EDX
	JMP j_out
j1:
	MOV EAX, y 		; EAX = y
	MOVSX EDX, z
	ADD EAX, EDX
	JMP j_out
j2:
	MOVSX EAX, x		; EAX = x
	IMUL EAX, EAX 		; EDX:EAX = x2
	ADD EAX, 32 		; EAX = x2 + 32
	MOV EDX, EAX
	MOVSX EAX, x
	MOVSX ECX, z
	IMUL EAX, ECX
	SUB EDX, EAX
	MOV EAX, EDX
j_out:
	push EAX
	push offset format
	call crt_printf 	; Вывод результата на экран
	call crt__getch 	; Задержка ввода
	push 0
	call ExitProcess 	; Выход из программы
end start

