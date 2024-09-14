#include "stdio.h"
#include "multiboot2.h"
#include "elf64.h"


void bootmain(unsigned long magic, unsigned long addr) {
    bprintf("This is a 32-bit bootstrapper.\n");
    bprintf("Magic number: %x\n", magic);
    bprintf("Multiboot info address: %x\n", addr);    
    for(;;);
}
