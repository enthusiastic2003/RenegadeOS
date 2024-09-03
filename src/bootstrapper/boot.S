global start
section .mb2_text
extern preboot_check
extern mode_check
extern terminal_initialize
extern bootstrapper_asm_info

start:
    mov esp, stack_top
    call terminal_initialize
    call mode_check
    cmp eax,0
    je initialize_protected_mode
    jmp unhandled_state
    

initialize_protected_mode:
    mov eax, handle_protected
    push eax
    call bootstrapper_asm_info
    jmp end 

unhandled_state:
    mov eax, unhandled_data
    push eax
    call bootstrapper_asm_info
    jmp end

end:
    cli
    hlt


section .bss
stack_bottom:
    resb 16384  ; 4096 * 4
stack_top:

section .data
    handle_protected db '[MSG] Handling Protected', 0
    unhandled_data db '[MSG] Not in Protected Mode. State Not handled. Halting'