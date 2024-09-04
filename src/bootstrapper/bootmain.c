#include "stdio.h"
#include "multiboot2.h"

void bootmain(unsigned long magic, unsigned long addr) {
    bprintf("This is a 32-bit bootstrapper.\n");
    bprintf("Magic number: %x\n", magic);
    struct multiboot_tag *tag;
    unsigned size;
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        bprintf("Invalid magic number. Halting...\n");
        for(;;);
    }
  if (addr & 7)
    {
      bprintf ("Unaligned mbi: 0x%x\n", addr);
      return;
    }
    
    for(;;);
}
