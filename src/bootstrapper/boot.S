global start

section .bss
stack_bottom:
    resb 16384  ; Reserve 16KB for stack
stack_top:

section .data
    handle_protected db '[MSG] Handling Protected', 0
    unhandled_data db '[MSG] Not in Protected Mode. State Not handled. Halting', 0
    loaded_gdt db '[MSG] 32-bit bootstrapper gdt loaded', 0

section .text
extern preboot_check
extern mode_check
extern terminal_initialize
extern bootstrapper_asm_info
extern gdt_install
extern bootmain
extern clear_paging

start:
    cli

    ; Initialize stack pointer
    mov esp, stack_top
    mov ebp, stack_top
    push ebx
    push eax

    ; Call functions to initialize terminal and check mode
    call terminal_initialize
    call mode_check
    ; Check the return value in EAX
    cmp eax, 0
    ; If EAX is 0, jump to initialize_protected_mode
    je initialize_protected_mode
    ; Otherwise, handle unhandled state
    jmp unhandled_state

initialize_protected_mode:
    ; Set EAX to the address of handle_protected message
    mov eax, handle_protected
    push eax
    ; Call bootstrapper_asm_info with the message address
    call bootstrapper_asm_info
    add esp, 4

    ; Call gdt_install to install GDT
    call gdt_install
    ; Set EAX to the address of loaded_gdt message
    mov eax, loaded_gdt
    push eax
    ; Call bootstrapper_asm_info with the message address
    call bootstrapper_asm_info
    add esp, 4
    ; Jump to end
    ; Now, safe to call main c 

    call clear_paging

    call bootmain
    sti
    jmp end 

unhandled_state:
    ; Set EAX to the address of unhandled_data message
    mov eax, unhandled_data
    push eax
    ; Call bootstrapper_asm_info with the message address
    call bootstrapper_asm_info
    add esp, 4
    ; Jump to end
    jmp end

end:
    ; Disable interrupts and halt CPU
    cli
    hlt
