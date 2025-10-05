.686
.model flat, stdcall
option casemap: none

include		kernel32.inc
include 	msvcrt.inc
includelib	kernel32.lib
includelib	msvcrt.lib

.DATA
	a db 7Fh, 0A3h, 0C1h, 0B9h, 0E0h, 8Dh, 45h, 0F2h, 0A1h, 0B3h, 7Ch, 9Dh, 58h, 0E4h, 0A6h ; 15 байт
	b db 12h, 0D9h, 0A7h, 0F5h, 0C4h, 0E3h, 8Bh, 01h, 0FAh, 76h, 0D2h, 0B9h, 0C5h, 8Eh, 30h ; 15 байт

	r db 15 dup(?) ; Для результата резервируется 15 байт
.CODE
START:
	mov EAX, dword ptr a[0]
	sub EAX, dword ptr b[0]
	mov dword ptr r[0], EAX
	
	mov EAX, dword ptr a[4]
	sbb EAX, dword ptr b[4]
	mov dword ptr r[4], EAX

	mov EAX, dword ptr a[8]
	sbb EAX, dword ptr b[8]
	mov dword ptr r[8], EAX	
	
	mov AX, word ptr a[12]
	sbb AX, word ptr b[12]
	mov word ptr r[12], AX

	mov AL, byte ptr a[14]
	sbb AL, byte ptr b[14]
	mov byte ptr r[14], AL

	push 0
	call ExitProcess
END START