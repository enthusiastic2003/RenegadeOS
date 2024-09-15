#ifndef ELF64_H
#define ELF64_H

#include <stdint.h>

// ELF Identification indexes
#define EI_MAG0       0       // File identification
#define EI_MAG1       1
#define EI_MAG2       2
#define EI_MAG3       3
#define EI_CLASS      4       // File class
#define EI_DATA       5       // Data encoding
#define EI_VERSION    6       // File version
#define EI_OSABI      7       // OS/ABI identification
#define EI_ABIVERSION 8       // ABI version
#define EI_PAD        9       // Start of padding bytes
#define EI_NIDENT     16      // Size of e_ident[]

#define ELFMAG0       0x7f    // ELF magic number
#define ELFMAG1       'E'
#define ELFMAG2       'L'
#define ELFMAG3       'F'

#define ELFCLASS64    2       // 64-bit objects
#define ELFDATA2LSB   1       // Little-endian data encoding

// ELF File Types
#define ET_NONE   0           // No file type
#define ET_REL    1           // Relocatable file
#define ET_EXEC   2           // Executable file
#define ET_DYN    3           // Shared object file
#define ET_CORE   4           // Core file

// ELF Machine Types
#define EM_NONE   0           // No machine
#define EM_X86_64 62          // AMD x86-64 architecture

// ELF Program Header Types
#define PT_NULL    0          // Unused entry
#define PT_LOAD    1          // Loadable segment
#define PT_DYNAMIC 2          // Dynamic linking information
#define PT_INTERP  3          // Interpreter path name
#define PT_NOTE    4          // Auxiliary information
#define PT_SHLIB   5          // Reserved
#define PT_PHDR    6          // Program header table itself
#define PT_TLS     7          // Thread-local storage segment

// ELF Program Header Flags
#define PF_X       0x1        // Executable
#define PF_W       0x2        // Writable
#define PF_R       0x4        // Readable

// ELF Header for 64-bit
typedef struct {
    uint8_t  e_ident[EI_NIDENT];  // ELF identification
    uint16_t e_type;              // Object file type
    uint16_t e_machine;           // Machine type
    uint32_t e_version;           // Object file version
    uint64_t e_entry;             // Entry point address
    uint64_t e_phoff;             // Program header offset
    uint64_t e_shoff;             // Section header offset
    uint32_t e_flags;             // Processor-specific flags
    uint16_t e_ehsize;            // ELF header size
    uint16_t e_phentsize;         // Size of program header entry
    uint16_t e_phnum;             // Number of program header entries
    uint16_t e_shentsize;         // Size of section header entry
    uint16_t e_shnum;             // Number of section header entries
    uint16_t e_shstrndx;          // Section name string table index
} Elf64_Ehdr;

// Program Header for 64-bit
typedef struct {
    uint32_t p_type;   // Type of segment
    uint32_t p_flags;  // Segment attributes
    uint64_t p_offset; // Offset in file
    uint64_t p_vaddr;  // Virtual address in memory
    uint64_t p_paddr;  // Physical address (not used)
    uint64_t p_filesz; // Size of segment in file
    uint64_t p_memsz;  // Size of segment in memory
    uint64_t p_align;  // Alignment of segment
} Elf64_Phdr;

#endif // ELF64_H
