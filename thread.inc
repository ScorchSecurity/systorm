; thread.inc
%ifndef _THREAD_INC
%define _THREAD_INC

%include "syscall.inc"
%include "utils.inc"

; consts
%define STACK_SIZE  (4096 * 1024)   ; 4 MB

%define PROT_READ       0x1
%define PROT_WRITE      0x2
%define PROT_EXEC       0x4
%define PROT_NONE       0x0
%define PROT_READWRITE  PROT_READ | PROT_WRITE

%define MAP_SHARED      0x1
%define MAP_PRIVATE     0x2
%define MAP_FIXED       0x10
%define MAP_FILE        0x0
%define MAP_ANONYMOUS   0x20
%define MAP_GROWSDOWN   0x100

%define CLONE_VM        0x100
%define CLONE_FS        0x200
%define CLONE_FILES     0x400
%define CLONE_SIGHAND   0x800
%define CLONE_PARENT    0x8000
%define CLONE_THREAD    0x10000
%define CLONE_IO        0x80000000
%define CLONE_ALL       CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_PARENT|CLONE_THREAD|CLONE_IO

; macros

; create thread
%macro thread.create 1                  ; ptr to code to run in seperate thread
stackpatch_start
    push %1
    call thread
stackpatch_end
%endmacro

; join thread
%macro thread.join 1
    sys.waitpid %1, 0, 0
%endmacro

; fork thread
%macro thread.fork 1
    push %1
    call create_fthread
%endmacro


; functions
create_fthread:
    sys.fork
    cmp eax, 0                          ; if eax == 0, then this is the child process
    jz jmp_to_ptr
    ret

; TODO: implement ability to add arguments to threaded function
thread:                                 ; thanks to "http://nullprogram.com/blog/2015/05/15/".
    call .create_stack                  ; addr of stack in eax
    lea ecx, [eax + STACK_SIZE - 4]     ; ebx should now be the top of the new stack
    pop dword [ecx]                     ; pop func-to-thread onto top of new stack
    ; therefore, when eip hits ret, it will return to the new function 
    sys.clone CLONE_ALL, ecx, 0, 0, 0   ; returns 0 if this is the new thread
    ret
.create_stack:                          ; push args onto stack in reverse order
    sys.mmap2 0, STACK_SIZE, PROT_READWRITE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_GROWSDOWN
    ret

jmp_to_ptr:                             ; jmp to register
    mov eax, [esp+4]
    jmp eax

%endif  ; _THREAD_INC