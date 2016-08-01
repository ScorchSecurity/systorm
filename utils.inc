%ifndef _UTILS_INC
%define _UTILS_INC

%include "syscall.inc"

; functions

; sbrk
_sbrk_:           
    sys.brk ebx
    add eax, ebx
    sys.brk eax
    ret
; memcpy
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

%endif  ; _UTILS_INC