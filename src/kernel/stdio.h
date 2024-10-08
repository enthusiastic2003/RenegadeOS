#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint8_t vga_entry_color(enum vga_color , enum vga_color );
static inline uint16_t vga_entry(unsigned char , uint8_t );
size_t strlen(const char* );
void terminal_initialize(void);
void terminal_setcolor(uint8_t );
void terminal_putentryat(char , uint8_t , size_t , size_t );
void terminal_putchar(char );
void terminal_write(const char* , size_t );
void terminal_writestring(const char* );
void bootstrapper_asm_info(const char* str);
void bprintf(const char* , ...);
void cls(void);
