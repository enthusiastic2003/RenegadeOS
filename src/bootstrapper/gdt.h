#ifndef GDT_H
#define GDT_H

#include <stdint.h>  // For standard integer types

// Access Byte Constants for GDT Entries
// Segment is present
#define GDT_PRESENT        0x80

// Descriptor Privilege Levels (DPL)
#define GDT_PRIVILEGE_0    0x00  // Ring 0
#define GDT_PRIVILEGE_1    0x20  // Ring 1
#define GDT_PRIVILEGE_2    0x40  // Ring 2
#define GDT_PRIVILEGE_3    0x60  // Ring 3

// Code Segment Access Types
#define GDT_CODE_EXEC_READ 0x9A  // Executable, readable, and present (ring 0)
#define GDT_CODE_EXEC       0x9A  // Executable and present (ring 0)
#define GDT_CODE_READ       0x92  // Readable and present (ring 0)

// Data Segment Access Types
#define GDT_DATA_READ_WRITE 0x92  // Readable, writable, and present (ring 0)
#define GDT_DATA_WRITE      0x92  // Writable and present (ring 0)
#define GDT_DATA_READ       0x90  // Readable and present (ring 0)

// System Segment Access Types
#define GDT_TSS_AVAILABLE   0x89  // Task State Segment (available, present, ring 0)
#define GDT_TSS_BUSY        0x8B  // Task State Segment (busy, present, ring 0)

// Granularity Byte Constants (Granularity is usually part of the Granularity/Flags byte)
#define GDT_GRANULARITY_4K 0x80  // 4KB Granularity
#define GDT_GRANULARITY_1B 0x00  // 1 Byte Granularity (default)

// Set the Granularity Byte (combines with access rights byte)
#define GDT_GRANULARITY_MASK 0xCF  // Mask for granularity bits

// Other constants if needed
#define GDT_TYPE_CODE_DATA  0x10  // Code/Data segment type
#define GDT_TYPE_TSS        0x09  // Task State Segment type

// Define the GDT entry structure
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

// Define the GDT pointer structure
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Externally declared GDT pointer
extern struct gdt_ptr gp;

// Function prototypes
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_install();
#endif // GDT_H
