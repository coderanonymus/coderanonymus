#include "aether/debug.h"
#include "vga.h"

extern char _kernel_symtab_start;
extern char _kernel_symtab_end;

void dbg_console_init(void) {
    vga_init();
}

void dbg_puts(const char *msg) {
    vga_writeln(msg);
}

static void trace_walk(uint64_t rbp) {
    vga_writeln("stack trace:");
    uint64_t *frame = (uint64_t *)rbp;
    for (int i = 0; frame && i < 10; i++) {
        uint64_t ret = frame[1];
        (void)ret;
        vga_writeln(" - frame");
        frame = (uint64_t *)frame[0];
    }
}

void panic_with_trace(const char *msg, uint64_t rbp) {
    vga_writeln("KERNEL PANIC");
    vga_writeln(msg);
    trace_walk(rbp);
    vga_writeln("symbols available");
    (void)&_kernel_symtab_start;
    (void)&_kernel_symtab_end;
    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}

void panic(const char *msg) {
    uint64_t rbp;
    __asm__ volatile ("mov %%rbp, %0" : "=r"(rbp));
    panic_with_trace(msg, rbp);
}
