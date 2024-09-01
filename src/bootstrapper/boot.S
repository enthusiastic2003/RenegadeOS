global start
section .mb2_text
extern preboot_check
start:
    lea esp, [stack_top]
    ; print `OK`
    call preboot_check
    mov dword [0xb8000], 0x2f4b2f4f
    hlt

section .bss
stack_bottom:
    resb 16384  ; 4096 * 4
stack_top: