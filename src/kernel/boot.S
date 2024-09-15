global start

section .bss
stack_bottom:
    resb 16384  ; Reserve 16KB for stack
stack_top:

section .bss align=4096
boot_page_directory resb 4096
boot_page_table1 resb 4096


section .data
    handle_protected db '[MSG] Handling Protected', 0
    unhandled_data db '[MSG] Not in Protected Mode. State Not handled. Halting', 0
    loaded_gdt db '[MSG] 32-bit bootstrapper gdt loaded', 0

section .mb2_text
extern preboot_check
extern mode_check
extern terminal_initialize
extern bootstrapper_asm_info
extern gdt_install
extern bootmain
extern clear_paging
extern _kernel_end
extern _kernel_start
global boot_page_directory

start:
    cli

    mov edi, (boot_page_table1 - 0xC0000000)  ; Load physical address of boot_page_table1
    mov esi, 0                                ; Start at address 0
    mov ecx, 1023                                ; Map 1023 pages

map_loop:
    cmp esi, 0                ; Check if mapping kernel
    jl skip                               ; Skip if not in kernel range
    cmp esi, (0x3fffff)   
    jge end_map                           ; Stop if past kernel

    mov edx, esi                          ; Map physical address
    or edx, 0x003                         ; Mark as present, writable
    mov [edi], edx

skip:
    add esi, 4096                         ; Move to next page
    add edi, 4                            ; Move to next entry
    loop map_loop                         ; Repeat for all pages

end_map:

    mov dword [boot_page_table1 - 0xC0000000 + 1023 * 4], (0x000B8000 | 0x003) ; Map VGA memory

    ; Map page table to virtual addresses 0x00000000 and 0xC0000000
    mov dword [boot_page_directory - 0xC0000000 + 0], (boot_page_table1 - 0xC0000000 + 0x003)
    mov dword [boot_page_directory - 0xC0000000 + 768 * 4], (boot_page_table1 - 0xC0000000 + 0x003)

    

    ; Load page directory into CR3
    mov ecx, (boot_page_directory - 0xC0000000)
    mov cr3, ecx
    
    
    
    ; Enable paging and write-protect
    mov ecx, cr0
    or ecx, 0x80010000
    mov cr0, ecx


   ; Jump to higher half with an absolute jump.
lea ecx, [fantasy_high]
jmp ecx

section .text

fantasy_high:
    ; At this point, paging is fully set up and enabled.

    ; Unmap the identity mapping as it is now unnecessary.
    mov dword [boot_page_directory], 0

    ; Reload CR3 to force a TLB flush so the changes take effect.
    mov ecx, cr3
    mov cr3, ecx

    ; Set up the stack.
    mov esp, stack_top
    mov ebp, stack_top

    add ebx, 0xC0000000
    
    push ebx
    push eax

    call terminal_initialize

    mov eax, loaded_gdt
    push eax
    call bootstrapper_asm_info
    add esp,4 

    call bootmain 

    cli
l2:  hlt
    jmp short l2


end:
    ; Disable interrupts and halt CPU
    cli
    hlt