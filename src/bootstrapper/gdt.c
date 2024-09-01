#include "gdt.h"

struct gdt_entry_t gdt[3] __attribute__((aligned(8))); // Align GDT to 8 bytes
struct gdt_ptr_t gp __attribute__((aligned(8)));       // Align GDT pointer to 8 bytes

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void gdt_install() {
    gp.limit = (sizeof(struct gdt_entry_t) * 6) - 1; // Limit is size of GDT entries - 1
    gp.base = (uint64_t)&gdt; // Base address of the GDT (64-bit)

    // Null Segment
    gdt_set_gate(0, 0, 0, 0, 0);

    // Kernel Code Segment (64-bit, ring 0)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Use 0xCF for 64-bit code segment granularity

    // Kernel Data Segment (64-bit, ring 0)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Use 0xCF for 64-bit data segment granularity

    // User Code Segment (64-bit, ring 3)
    // Note: Ensure to define a GDT setup function to load the GDT register
    // Example: gdt_load();
}
