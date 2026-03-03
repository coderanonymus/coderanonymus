#ifndef AETHER_ARCH_H
#define AETHER_ARCH_H

#include <stdint.h>

void arch_init(void);
void arch_hpet_init(void);
void arch_ioapic_init(void);
void arch_msi_init(void);
void arch_usb_init(void);
uint64_t arch_time_ns(void);

#endif
