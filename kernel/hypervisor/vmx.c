#include "aether/log.h"
#include <stdint.h>

int vmx_probe_and_enable(void) {
    uint32_t eax = 1, ebx = 0, ecx = 0, edx = 0;
    __asm__ volatile (
        "cpuid"
        : "+a"(eax), "=b"(ebx), "+c"(ecx), "=d"(edx)
    );
    if ((ecx & (1u << 5)) == 0) {
        return 0;
    }
    log_info("hv/vmx: VMX capability detected");
    return 1;
}
