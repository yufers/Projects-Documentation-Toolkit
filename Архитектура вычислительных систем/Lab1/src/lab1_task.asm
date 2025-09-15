.686
.model flat, stdcall
option casemap: none

include windows.inc
include kernel32.inc
include msvcrt.inc
include user32.inc
includelib	kernel32.lib
includelib	msvcrt.lib
includelib 	user32.lib

.DATA
	a DD 30201, 30201h
	b DB 43h, 0F3h, 0F3h, 0E5h 
	;DF 1500
	;DD 1.5, 1.6, 1.9, -1.9
	;t DQ 0E7D32A1h
	;stra DB 16 dup(1)
	
	str1 db "some string 1", 0

	str_fmt db "%s", 9, "%p", 9, "%p", 9, "%d", 13, 10, "%s", 9, "%p", 9, "%p", 9, "%d", 13, 10, 0
	str1_name db "str1", 0
	a_name db "a", 0
	b_name db "b", 0
	out_buffer db 256 dup(0)
	caption db "information", 0
.CODE
START:
	;b
	push 4
	push offset b + 3 
	push offset b 
	push offset b_name
	
	;a
	push 8
	push offset a + 7 
	push offset a 
	push offset a_name
	push offset str_fmt

	call crt_printf

	push offset out_buffer	

	call crt_sprintf
	add esp, 4*9

	push 0
	push offset caption 
	push offset out_buffer
	push 0
	call MessageBoxA
	add esp, 4*4

	push 0
	call ExitProcess
END START
