#ifndef AETHER_HYPERVISOR_H
#define AETHER_HYPERVISOR_H

#include <stdint.h>
#include "aether/types.h"

typedef enum {
    HV_BACKEND_NONE = 0,
    HV_BACKEND_VMX,
    HV_BACKEND_SVM,
} hv_backend_t;

typedef struct {
    uint64_t guest_rip;
    uint64_t guest_rsp;
    uint64_t guest_cr3;
    uint64_t eptp_or_npt_root;
} hv_guest_ctx_t;

void hv_init(void);
hv_backend_t hv_backend(void);
aether_status_t hv_launch_guest(hv_guest_ctx_t *ctx);
void hv_vmexit_dispatch(uint64_t reason);

#endif
