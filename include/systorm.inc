; systorm.inc

%include "syscall.inc"
%include "socket.inc"
%include "utils.inc"
%include "thread.inc"

; use macro
%macro use 1-*
    %rep %0
        use.%1
    %rotate 1
    %endrep
%endmacro