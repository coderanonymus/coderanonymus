#ifndef AETHER_DEBUG_H
#define AETHER_DEBUG_H

#include <stdint.h>

void dbg_console_init(void);
void dbg_puts(const char *msg);
void panic(const char *msg);
void panic_with_trace(const char *msg, uint64_t rbp);

#endif
