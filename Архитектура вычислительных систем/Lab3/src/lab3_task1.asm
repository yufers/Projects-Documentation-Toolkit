.686
.model flat, stdcall
option casemap: none

include		kernel32.inc
include 	msvcrt.inc
includelib	kernel32.lib
includelib	msvcrt.lib

.DATA
	x db -100
	y db 10
	z dw -40		
	str_fmt db "x = %d, y = %d, z = %hd, R = %d", 0
.CODE
START:
	movsx EAX, x            ; Расширение байта x до двойного слова в EAX
	add EAX, 10             ; EAX = EAX + 10
	mov EBX, EAX		; EBX = EAX

	movsx EAX, y            ; Расширение байта y до двойного слова в EAX
	sub EAX, 5              ; EAX = EAX - 5
	mul EBX			; EAX = EAX * EBX
	mov EBX, EAX		; EBX = EAX

	movsx EAX, z            ; Расширение слова z до двойного слова в EAX
	mov ECX, 3              ; ECX = 3
	cdq                     ; Расширение до двойного слова
	idiv ECX                ; EAX = EDX:EAX / ECX(3)
	mov EDX, EAX            ; EDX = EAX
	movsx EAX, z            ; Расширение слова z до двойного слова в EAX
	sub EAX, EDX            ; EAX = EAX - EDX
	mul EBX                 ; EAX = EAX * EBX
	
	sub EAX, 7*7*7*7	; EAX = EAX - 2401			      

	; Вызов функции printf("x = %d, y = %d, z = %hd, R = %d", (int)x, (int)y, z, EAX)
	push EAX 		; Поместим в стек итоговый результат. Выводим его как 4-байтовое со спецификатором %d
	push dword ptr z 	; Для числа z типа short используем спецификатор %hd
	movsx EAX, x		; EAX = (int)x. Число x расширим до 4-байтового и используем для него спецификатор %d
	push EAX
	movsx EAX, y		; EAX = (int)y. Число y расширим до 4-байтового и используем для него спецификатор %d
	push EAX
	push offset str_fmt
	call crt_printf
	add ESP, 5*4 		; Очистка стека от аргументов
  
	push 0
	call ExitProcess 	; Выход из программы
END START
