#include "vga.h"
#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;
static size_t row = 0;
static size_t col = 0;
static const uint8_t color = 0x0F;

static void vga_disable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

static void scroll_if_needed(void) {
    if (row < VGA_HEIGHT) {
        return;
    }

    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA[(y - 1) * VGA_WIDTH + x] = VGA[y * VGA_WIDTH + x];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        VGA[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)color << 8 | ' ';
    }

    row = VGA_HEIGHT - 1;
}

static void newline(void) {
    col = 0;
    row++;
    scroll_if_needed();
}

void vga_init(void) {
    vga_disable_cursor();
}

void vga_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA[y * VGA_WIDTH + x] = (uint16_t)color << 8 | ' ';
        }
    }
    row = 0;
    col = 0;
}

void vga_putc(char c) {
    if (c == '\n') {
        newline();
        return;
    }

    if (c == '\b') {
        if (col > 0) {
            col--;
            VGA[row * VGA_WIDTH + col] = (uint16_t)color << 8 | ' ';
        }
        return;
    }

    VGA[row * VGA_WIDTH + col] = (uint16_t)color << 8 | (uint8_t)c;
    col++;
    if (col >= VGA_WIDTH) {
        newline();
    }
}

void vga_write(const char *s) {
    for (; *s; ++s) {
        vga_putc(*s);
    }
}

void vga_writeln(const char *s) {
    vga_write(s);
    vga_putc('\n');
}
