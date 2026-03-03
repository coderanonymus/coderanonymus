#ifndef AETHER_SCHED2_H
#define AETHER_SCHED2_H

#include <stdint.h>

void sched2_init(void);
void sched2_tick(uint64_t now_ns);

#endif
