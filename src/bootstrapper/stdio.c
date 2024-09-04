#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"
#include <stdarg.h>
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

/* This tutorial will only work for the 32-bit ix86 targets. */
//#if !defined(__i386__)
//#error "This tutorial needs to be compiled with a ix86-elf compiler"
//#endif

/* Hardware text mode color constants. */

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) 
{
	if(c == '\n') {
		terminal_column = 0;
		if (terminal_row == VGA_HEIGHT -1)
			terminal_row = 0;
		else
			terminal_row++;
		return;
	}

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void bootstrapper_asm_info(const char* str) 
{
	
	terminal_writestring(str);
	terminal_writestring("\n");

	return;
}

// Helper functions for integer to string conversion
static void itoa(int value, char* str, int base) {
    static const char digits[] = "0123456789abcdef";
    char* p = str;
    int num = value;
    
    // Handle special case of 0
    if (value == 0) {
        *p++ = '0';
        *p = '\0';
        return;
    }
    
    // Handle negative numbers
    if (num < 0 && base == 10) {
        *p++ = '-';
        num = -num;
    }
    
    // Convert to string
    char* start = p;
    do {
        *p++ = digits[num % base];
        num /= base;
    } while (num > 0);
    
    *p = '\0';
    
    // Reverse string
    char* end = p - 1;
    while (start < end) {
        char tmp = *start;
        *start++ = *end;
        *end-- = tmp;
    }
}

void bprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    const char* p;
    char buffer[64];
    
    for (p = format; *p != '\0'; p++) {
        if (*p != '%') {
            terminal_putchar(*p);
            continue;
        }
        
        p++;
        switch (*p) {
            case 's': {
                const char* str = va_arg(args, const char*);
                terminal_writestring(str);
                break;
            }
            case 'd': {
                int value = va_arg(args, int);
                itoa(value, buffer, 10);
                terminal_writestring(buffer);
                break;
            }
            case 'u': {
                unsigned int value = va_arg(args, unsigned int);
                itoa(value, buffer, 10);
                terminal_writestring(buffer);
                break;
            }
            case 'x': {
                unsigned int value = va_arg(args, unsigned int);
                itoa(value, buffer, 16);
                terminal_writestring(buffer);
                break;
            }
            case 'c': {
                char value = (char) va_arg(args, int);
                terminal_putchar(value);
                break;
            }
            case 'p': {
                void* ptr = va_arg(args, void*);
                uintptr_t addr = (uintptr_t) ptr;
                itoa(addr, buffer, 16);
                terminal_writestring(buffer);
                break;
            }
            default:
                terminal_putchar('%');
                terminal_putchar(*p);
                break;
        }
    }
    
    va_end(args);
}

void cls(void) {
    terminal_row = 0;
    terminal_column = 0;
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}
