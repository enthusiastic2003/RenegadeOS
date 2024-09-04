global mode_check
global clear_paging
extern bootstrapper_asm_info
extern gpr

mode_check:
    push ebp
    mov ebp, esp

    ; Check if in 16-bit Real Mode
    mov eax, cr0
    test eax, 1
    jz in_real_mode

    ; Check if in Long Mode
    mov ecx, 0xC0000080       ; EFER MSR
    rdmsr
    test edx, 0x400           ; Check if Long Mode Enabled (LME)
    jnz in_long_mode

    ; If neither Real Mode nor Long Mode, assume Protected Mode
in_protected_mode:
    mov eax, protected_msg
    push eax
    call bootstrapper_asm_info
    add esp, 4                 ; Clean up the stack after the function call
    mov eax, 0                 ; Return code for Protected Mode
    jmp end_chk

in_real_mode:
    mov eax, real_msg
    push eax
    call bootstrapper_asm_info
    add esp, 4                 ; Clean up the stack after the function call
    mov eax, 1                 ; Return code for Real Mode
    jmp end_chk

in_long_mode:
    mov eax, long_64_msg
    push eax
    call bootstrapper_asm_info
    add esp, 4                 ; Clean up the stack after the function call
    mov eax, 2                 ; Return code for Long Mode
    jmp end_chk

end_chk:
    mov esp, ebp               ; Restore the stack pointer from the base pointer
    pop ebp                    ; Restore the old base pointer
    ret                        ; Return to the caller

clear_paging:
    mov edi, 0x1000    ; Set the destination index to 0x1000.
    mov cr3, edi       ; Set control register 3 to the destination index.
    xor eax, eax       ; Nullify the A-register.
    mov ecx, 4096      ; Set the C-register to 4096.
    rep stosd          ; Clear the memory.
    mov edi, cr3       ; Set the destination index to control register 3.
    ret


section .data
    protected_msg db '[MSG] Detected Mode: Protected Mode', 0
    real_msg db '[MSG] Detected Mode: Real Mode\n', 0
    long_64_msg db '[MSG] Detected Mode: Long Mode (64-bit)\n', 0
