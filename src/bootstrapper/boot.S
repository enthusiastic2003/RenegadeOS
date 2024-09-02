global start
section .mb2_text
extern preboot_check
extern mode_check
extern terminal_initialize

start:
    mov esp, stack_top
    call terminal_initialize
    call mode_check
    hlt

section .bss
stack_bottom:
    resb 16384  ; 4096 * 4
stack_top: