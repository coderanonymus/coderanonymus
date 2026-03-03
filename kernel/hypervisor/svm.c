#include "aether/log.h"
#include <stdint.h>

int svm_probe_and_enable(void) {
    uint32_t eax = 0x80000001u, ebx = 0, ecx = 0, edx = 0;
    __asm__ volatile (
        "cpuid"
        : "+a"(eax), "=b"(ebx), "+c"(ecx), "=d"(edx)
    );
    if ((ecx & (1u << 2)) == 0) {
        return 0;
    }
    log_info("hv/svm: SVM capability detected");
    return 1;
}
