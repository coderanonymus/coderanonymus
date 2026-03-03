#ifndef AETHER_VGA_H
#define AETHER_VGA_H

#include <stddef.h>
#include <stdint.h>

void vga_init(void);
void vga_clear(void);
void vga_putc(char c);
void vga_write(const char *s);
void vga_writeln(const char *s);

#endif
