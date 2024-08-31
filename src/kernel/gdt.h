// gdt.h
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry_t {
    uint16_t limit_low;      // Lower 16 bits of limit
    uint16_t base_low;       // Lower 16 bits of base
    uint8_t  base_middle;    // Next 8 bits of base
    uint8_t  access;         // Access flags
    uint8_t  granularity;    // Granularity and high 4 bits of limit
    uint8_t  base_high;      // High 8 bits of base
} __attribute__((packed));

struct gdt_ptr_t {
    uint16_t limit;          // Size of GDT
    uint32_t base;           // Base address of GDT
} __attribute__((packed));

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_install();

#endif // GDT_H
