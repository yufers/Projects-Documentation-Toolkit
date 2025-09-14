.686
 .model flat, stdcall
 option casemap: none

 include windows.inc
 include kernel32.inc
 include msvcrt.inc
 includelib	kernel32.lib
 includelib	msvcrt.lib

.DATA
	a DD 30201, 30201h
	b DB 43h, 0F3h, 0F3h, 0E5h 
	DF 1500
	DD 1.5, 1.6, 1.9, -1.9
	t DQ 0E7D32A1h
	stra DB 16 dup(1)
.CODE
START:
	MOV ESI, 65737341h		; ESI = 65737341h	
	AND ESI, dword ptr b		; ESI = ESI & E5F3F343(т.к. DB просто определяет байты, а мы обращаемся к DD поэтому нужно развернуть)
	MOV dword ptr stra, ESI		; stra[0:3] = ESI (41737365h)
	MOV ECX, dword ptr t		; ECX = t[0:3] (0E7D32A1h)
	IMUL ECX, 7			; ECX = ECX * 7 (656C6267h)
	ADD ECX, 6			; ECX = ECX + 6 (656C626D)
	MOV dword ptr stra[4], ECX	; stra[4:7] = ECX (6D626C65h)
	ADD stra[8], 'q'		; stra[8] = 71h
	DEC stra[9]			; stra[9] -= 1 (0h)
	
	push offset stra
	call crt_puts			; puts(str1)
	ADD ESP, 4			; Очистка стека от аргумента
	
	call crt__getch 		; Задержка ввода, getch()
	; Вызов функции ExitProcess(0)
	push 0				; Поместить аргумент функции в стек
	call ExitProcess 		; Выход из программы
END START