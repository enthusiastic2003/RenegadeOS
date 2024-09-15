#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

// Define constants for page directory and page table entry attributes
#define PAGE_PRESENT 0x1
#define PAGE_READ_WRITE 0x2
#define PAGE_USER 0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_NO_CACHE 0x10
#define PAGE_ACCESSED 0x20
#define PAGE_DIRTY 0x40

// Typedefs for page directory and page table entries
typedef uint32_t page_directory_entry_t;
typedef uint32_t page_table_entry_t;

// Define a structure for the page directory
typedef struct {
    page_directory_entry_t entries[1024];  // 1024 entries, each 4 bytes
} page_directory_t;

// Define a structure for the page table
typedef struct {
    page_table_entry_t entries[1024];  // 1024 entries, each 4 bytes
} page_table_t;

// Function prototypes

/**
 * Initialize the paging system with the given address of the page directory.
 * @param pd_addr Address of the page directory.
 */
// void init_paging(uint32_t pd_addr);

/**
 * Set a page directory entry.
 * @param pd Address of the page directory.
 * @param index Index of the page directory entry.
 * @param value Value to set.
 */
// void set_page_directory_entry(page_directory_t* pd, uint32_t index, page_directory_entry_t value);

/**
 * Get a page directory entry.
 * @param pd Address of the page directory.
 * @param index Index of the page directory entry.
 * @return Value of the page directory entry.
 */
// page_directory_entry_t get_page_directory_entry(page_directory_t* pd, uint32_t index);

/**
 * Set a page table entry in the given page directory.
 * @param pd Address of the page directory.
 * @param pd_index Index of the page directory entry.
 * @param pt_index Index of the page table entry.
 * @param value Value to set.
 */
// void set_page_table_entry(page_directory_t* pd, uint32_t pd_index, uint32_t pt_index, page_table_entry_t value);

/**
 * Get a page table entry from the given page directory.
 * @param pd Address of the page directory.
 * @param pd_index Index of the page directory entry.
 * @param pt_index Index of the page table entry.
 * @return Value of the page table entry.
 */
// page_table_entry_t get_page_table_entry(page_directory_t* pd, uint32_t pd_index, uint32_t pt_index);

/**
 * Load the page directory register with the address of the page directory.
 * @param pd_addr Address of the page directory to load.
 */
// void load_page_directory(uint32_t pd_addr);

#endif // PAGING_H
