%ifndef _UTILS_INC
%define _UTILS_INC

%include "syscall.inc"

; functions

; sbrk

%macro use.utils.sbrk 0
_sbrk_:           
    sys.brk ebx
    add eax, ebx
    sys.brk eax
    ret
%endmacro

; memcpy
%macro use.utils.memcpy 0
_memcpy_:
    pop edi         ; ret
    pop eax         ; dest
    pop ebx         ; src
    pop edx         ; size
    xor ecx, ecx    ; zero out ecx
.loop:
    movzx esi, byte [ebx+ecx]   ; I had to use two instructions to move the data
    mov [eax+ecx], esi
    inc ecx
    cmp ecx, edx    ; cmp counter to size
    jnz .loop
    push edi
    ret
%endmacro

; macro wrappers
%macro sbrk 1     ; size
stackpatch_start
    mov ebx, %1
    call _sbrk_
stackpatch_end
%endmacro

%macro memcpy 3     ; dest, src, size
stackpatch_start
    push %3
    push %2
    push %1
    call _memcpy_
stackpatch_end
%endmacro

; stack frame stuff
%macro stackframe 1
    push ebp
    mov ebp, esp
    sub esp, %1
%endmacro

; embed string in code
%macro inline 1-*   ; moves addr of string into eax
    call %%after_string
        %rep %0
            db %1
        %rotate 1
        %endrep
    %%after_string:
        pop eax
%endmacro

%endif  ; _UTILS_INC