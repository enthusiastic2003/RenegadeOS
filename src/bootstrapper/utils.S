global mode_check
extern bootstrapper_asm_info

mode_check:
    ; Check if in 16-bit Real Mode
    mov eax, cr0
    test eax, 1
    jz in_real_mode

    ; Check if in Long Mode
    mov ecx, 0xC0000080       ; EFER MSR
    rdmsr
    test edx, 0x400           ; Check if Long Mode Enabled (LME)
    jnz in_long_mode

in_protected_mode:
    mov eax, protected_msg
    push eax
    call bootstrapper_asm_info
    add esp, 4
    mov eax, 0                ; Clean up stack after function call
    ret

in_real_mode:
    mov eax, real_msg
    push eax
    call bootstrapper_asm_info
    add esp, 4
    mov eax, 1
    ret

in_long_mode:
    mov eax, long_64_msg
    push eax
    call bootstrapper_asm_info
    add esp, 4
    mov eax, 2
    ret

section .data
    protected_msg db '[MSG] Detected Mode: Protected Mode', 0
    real_msg db '[MSG] Detected Mode: Real Mode\n', 0
    long_64_msg db '[MSG] Detected Mode: Long Mode (64-bit)\n', 0
