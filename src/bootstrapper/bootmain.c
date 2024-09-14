#include "stdio.h"
#include "multiboot2.h"
#include "elf64.h"

extern int _kernel_end;

void bootmain(unsigned long magic, unsigned long addr) {
    int var = 10;
    int* any_adr = &var;
    bprintf("This is a 32-bit bootstrapper.\n");
    bprintf("Magic number: %x\n", magic);
    bprintf("Multiboot info address: %x\n", addr);    
    bprintf("New pointer: %p\n", (void*)any_adr);
    bprintf("Variable: %d\n", var);
    bprintf("Kernel end: %p\n", &_kernel_end);
    bprintf("-var: %d\n", -var);
    for(;;);
}
