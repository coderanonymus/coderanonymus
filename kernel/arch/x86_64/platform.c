#include "aether/arch.h"
#include "aether/log.h"

static uint64_t fake_time;

void arch_hpet_init(void) {
    log_info("arch: HPET timer init (foundation)");
}

void arch_ioapic_init(void) {
    log_info("arch: IOAPIC routing init (foundation)");
}

void arch_msi_init(void) {
    log_info("arch: MSI capability init (foundation)");
}

void arch_usb_init(void) {
    log_info("arch: USB core init (foundation)");
}

void arch_init(void) {
    arch_hpet_init();
    arch_ioapic_init();
    arch_msi_init();
    arch_usb_init();
}

uint64_t arch_time_ns(void) {
    fake_time += 1000000;
    return fake_time;
}
