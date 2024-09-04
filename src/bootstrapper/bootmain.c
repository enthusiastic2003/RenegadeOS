#include "stdio.h"
#include "multiboot2.h"
#include "elf64.h"

void print_kernel_info(void* elf_start) {
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)elf_start;
    Elf64_Phdr *phdrs = (Elf64_Phdr *)(elf_start + elf_header->e_phoff);
    bprintf("  Entry Point: 0x%llx\n", elf_header->e_entry);
    bprintf("  Program Header Offset: 0x%llx\n", elf_header->e_phoff);
    bprintf("  Number of Program Headers: %d\n", elf_header->e_phnum);

    // Print Program Headers information
    bprintf("\nProgram Headers:\n");
    for (int i = 0; i < elf_header->e_phnum; i++) {
        Elf64_Phdr *phdr = &phdrs[i];
        if (phdr->p_type == 1) { // PT_LOAD
            bprintf("  Segment %d:\n", i);
            bprintf("    Type:         0x%x\n", phdr->p_type);
            bprintf("    Virtual Addr: 0x%p\n", phdr->p_vaddr);
            bprintf("    Physical Addr:0x%p\n", phdr->p_paddr);
            bprintf("    File Size:    0x%p\n", phdr->p_filesz);
            bprintf("    Memory Size:  0x%p\n", phdr->p_memsz);
            bprintf("    Flags:        0x%x\n", phdr->p_flags);
            bprintf("    Alignment:    0x%p\n", phdr->p_align);
        }
    }
}



void bootmain(unsigned long magic, unsigned long addr) {
    bprintf("This is a 32-bit bootstrapper.\n");
    bprintf("Magic number: %x\n", magic);
    struct multiboot_tag *tag;
    
    void* kernel64addr; 

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
    for (tag = (struct multiboot_tag *) (addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
      switch(tag->type){
        case MULTIBOOT_TAG_TYPE_CMDLINE:
          bprintf("Command line: %s\n", ((struct multiboot_tag_string *)tag)->string);
          break;
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
          bprintf("Boot loader name: %s\n", ((struct multiboot_tag_string *)tag)->string);
          break;
        case MULTIBOOT_TAG_TYPE_MODULE:
          bprintf("Module at 0x%x-0x%x. Command line: %s\n",
                  ((struct multiboot_tag_module *)tag)->mod_start,
                  ((struct multiboot_tag_module *)tag)->mod_end,
                  ((struct multiboot_tag_module *)tag)->cmdline);
                  kernel64addr = (void* ) ((struct multiboot_tag_module *)tag)->mod_start ;
          break;
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
          bprintf("mem_lower = %uKB, mem_upper = %uKB\n",
                  ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower,
                  ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper);
          break;
        case MULTIBOOT_TAG_TYPE_BOOTDEV:
          bprintf("Boot device 0x%x,%u,%u\n",
                  ((struct multiboot_tag_bootdev *)tag)->biosdev,
                  ((struct multiboot_tag_bootdev *)tag)->slice,
                  ((struct multiboot_tag_bootdev *)tag)->part);
          break;
          default:
            break;
      }

    }
    print_kernel_info(kernel64addr);
    bprintf("Paging enabled.\n");
    for(;;);
}
