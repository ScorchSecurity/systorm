; syscall.inc
%ifndef _SYSCALL_INC
%define _SYSCALL_INC

; const defines
%define stdout 2
%define stdin 1

; SYSCALL defines
%define SYS_EXIT 1
%define SYS_FORK 2
%define SYS_READ 3
%define SYS_WRITE 4
%define SYS_OPEN 5
%define SYS_CLOSE 6
%define SYS_WAITPID 7
%define SYS_CREAT 8
%define SYS_LINK 9
%define SYS_UNLINK 10
%define SYS_EXECVE 11
%define SYS_BRK 45
%define SYS_DUP2 63
%define SYS_SOCKET 102
%define SYS_CLONE 120
%define SYS_MPROTECT 125
%define SYS_MMAP2 192

%define syscall int 80h

%macro stackpatch_start 0
%ifdef CLEAN_STACK
    pushad
%endif
%endmacro
%macro stackpatch_end 0
%ifdef CLEAN_STACK
    mov [esp+28], eax
    popad
%endif
%endmacro

; raw syscall macros
; CLEAN_STACK is a bit broken for sys.clone and doesn't need to be enabled

%macro sys.write 3
stackpatch_start
    mov edx, %3
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_WRITE
    syscall             ; syscall is a macro that resolves to "int 80h"
stackpatch_end
%endmacro

%macro sys.exit 1       ; doesn't need to restore the stack
    mov eax, SYS_EXIT
    mov ebx, %1
    syscall
%endmacro

%macro sys.fork 0
stackpatch_start
    mov eax, SYS_FORK
    syscall
stackpatch_end
%endmacro

%macro sys.read 3
stackpatch_start
    mov edx, %3
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_READ
    syscall
stackpatch_end
%endmacro

%macro sys.execve 3
stackpatch_start
    mov edx, %3
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_EXECVE
    syscall
stackpatch_end
%endmacro

%macro sys.brk 1
stackpatch_start
    mov ebx, %1
    mov eax, SYS_BRK
    syscall
stackpatch_end
%endmacro

%macro sys.dup2 2
stackpatch_start
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_DUP2
    syscall
stackpatch_end
%endmacro

%macro sys.mmap2 4
stackpatch_start
    mov esi, %4
    mov edx, %3
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_MMAP2
    syscall
stackpatch_end
%endmacro

%macro sys.clone 5
stackpatch_start
    mov edi, %5
    mov esi, %4
    mov edx, %3
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_CLONE
    syscall
stackpatch_end
%endmacro

%macro sys.mprotect 3
stackpatch_start
    mov edx, %3
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_MPROTECT
    syscall
stackpatch_end
%endmacro

%macro sys.waitpid 3
stackpatch_start
    mov edx, %3
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_WAITPID
    syscall
stackpatch_end
%endmacro

%macro sys.socketcall 2
stackpatch_start
    mov ecx, %2
    mov ebx, %1
    mov eax, SYS_SOCKET
    syscall
stackpatch_end
%endmacro

%macro sys.close 1
stackpatch_start
    mov ebx, %1
    mov eax, SYS_CLOSE
    syscall
stackpatch_end
%endmacro

%endif  ; _SYSCALL_INC