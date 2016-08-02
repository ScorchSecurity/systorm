; fork.asm

[BITS 32]

%include "syscall.inc"
; %include "thread.inc"
%include "socket.inc"
%include "utils.inc"

global _start
; _start goes first
_start:
    jmp main

; syscalls do not need to be included with 'use.*'
use.sock.socket
use.sock.connect

main:
	stackframe 4

	sock.socket AF_INET, SOCK_STREAM
	mov dword [ebp-4], eax
	sock.connect dword [ebp-4], AF_INET, 0x0101017F, 0xD204	; 127.0.0.1, 1234

	inline 'Hello!', 0xa			; places addr of 'Hello!\n' into eax
	
	sys.write dword [ebp-4], eax, 7	; write 'Hello!\n' to sockfd
	sys.close dword [ebp-4]			; close sockfd