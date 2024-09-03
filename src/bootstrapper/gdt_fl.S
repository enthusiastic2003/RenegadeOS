; gdt.asm

global gdt_flush
extern gp

; Set up the Global Descriptor Table (GDT)
gdt_flush:
    ; Load the GDTR register with the address of the GDT
    lgdt [gp]              ; Load the GDT pointer into GDTR
    
    ; Flush and reload the segment registers
    mov ax, 0x10            ; Selector for the data/code segment (index 1)
    mov ds, ax              ; Set the data segment register (DS)
    mov es, ax              ; Set the extra segment register (ES)
    mov fs, ax              ; Set the FS register
    mov gs, ax              ; Set the GS register
    mov ss, ax              ; Set the stack segment register (SS)
    
    ; Reload the code segment register (CS) with a far jump
    ; 0x08 is the code segment selector index
    jmp 0x08:flush_done    ; Far jump to ensure CS is reloaded
    
flush_done:
    ret                     ; Return from the function
