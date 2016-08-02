; socket.inc
%ifndef _SOCKET_INC
%define _SOCKET_INC

%include "syscall.inc"

; consts
%define SOCK_SOCKET         1
%define SOCK_BIND           2
%define SOCK_CONNECT        3
%define SOCK_LISTEN         4
%define SOCK_ACCEPT         5
%define SOCK_GETSOCKNAME    6
%define SOCK_GETPEERNAME    7
%define SOCK_SOCKETPAIR     8
%define SOCK_SEND           9
%define SOCK_RECV           10
%define SOCK_SENDTO         11
%define SOCK_RECVFROM       12

%define SOCK_STREAM         1
%define AF_INET             2

; macros
%macro sock.socket 2
stackpatch_start
    push 0h
    push %2
    push %1
    mov ecx, esp
    call _sock_socket
stackpatch_end
%endmacro

%macro sock.bind 3      ; fd, family (e.i. AF_INET), port (in network order; use htons)
stackpatch_start
    push %3
    push %2
    push %1
    call _sock_bind
stackpatch_end
%endmacro

%macro sock.listen 2    ; fd, backlog
stackpatch_start
    push %2
    push %1
    call _sock_listen
stackpatch_end
%endmacro

%macro sock.accept 1    ; fd
stackpatch_start
    push %1
    call _sock_accept
stackpatch_end
%endmacro

%macro sock.connect 4   ; fd, family, addr, port (addr & port need to be network byte order)
stackpatch_start
    push %4
    push %3
    push %2
    push %1
    call _sock_connect
stackpatch_end
%endmacro

; functions
%macro use.sock.socket 0
_sock_socket:
    sys.socketcall SOCK_SOCKET, ecx ; call socketcall with socket struct
    ret
%endmacro

%macro use.sock.bind 0
_sock_bind:
    stackframe 12    ; allocate stack frame for dword + word*2
    mov eax, [ebp+8]; mov fd into eax
    mov [ebp-4], eax; mov fd into [ebp-4]
    mov eax, [ebp+12]; mov family into eax
    mov [ebp-8], eax; mov family into [ebp-8]
    mov eax, [ebp+16]; mov port into eax
    mov [ebp-12], eax; mov port into [ebp-12]
    push 0x0
    push word [ebp-12]; port
    push word [ebp-8]; family
    mov edi, esp
    push 10h        ; sizeof(struct sockaddr_in)
    push edi        ; struct sockaddr_in
    push dword [ebp-4]; fd
    mov ecx, esp
    sys.socketcall SOCK_BIND, ecx
    leave           ; destroy stack frame
    ret
%endmacro

%macro use.sock.listen 0
_sock_listen:
    lea ecx, [esp+4]
    sys.socketcall SOCK_LISTEN, ecx
    ret
%endmacro

%macro use.sock.accept 0
_sock_accept:
    mov eax, [esp+4]
    push 0
    push 0
    push eax
    sys.socketcall SOCK_ACCEPT, esp
    add esp, 12
    ret
%endmacro

%macro use.sock.connect 0
_sock_connect:
    stackframe 16   ; allocate stack frame for dword*3
    mov eax, [ebp+8]; mov fd into eax
    mov [ebp-4], eax; mov fd into [ebp-4]
    mov eax, [ebp+12]; mov family into eax
    mov [ebp-8], eax; mov family into [ebp-8]
    mov eax, [ebp+16]; mov addr into eax
    mov [ebp-12], eax; mov addr into [ebp-12]
    mov eax, [ebp+20]; mov port into eax
    mov [ebp-16], eax; mov port into [ebp-16]
    push dword [ebp-12]; addr
    push word [ebp-16]; port
    push word [ebp-8]; family
    mov ecx, esp
    push byte 10h   ; sizeof(struct sockaddr_in)
    push ecx
    push dword [ebp-4]    ; fd
    sys.socketcall SOCK_CONNECT, esp
    leave           ; destroy stack frame
    ret
%endmacro

%endif  ; _SOCKET_INC