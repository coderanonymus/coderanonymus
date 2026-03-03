BITS 64
GLOBAL _start
EXTERN kmain

SECTION .text
_start:
    mov rsp, stack_top
    call kmain
.hang:
    hlt
    jmp .hang

SECTION .bss
align 16
stack_bottom:
    resb 16384
stack_top:
