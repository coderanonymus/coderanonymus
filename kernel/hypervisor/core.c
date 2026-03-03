#include "aether/hypervisor.h"
#include "aether/log.h"

extern int vmx_probe_and_enable(void);
extern int svm_probe_and_enable(void);

static hv_backend_t g_backend;

void hv_init(void) {
    g_backend = HV_BACKEND_NONE;
    if (vmx_probe_and_enable()) {
        g_backend = HV_BACKEND_VMX;
        log_info("hv: VMX enabled");
        return;
    }
    if (svm_probe_and_enable()) {
        g_backend = HV_BACKEND_SVM;
        log_info("hv: SVM enabled");
        return;
    }
    log_warn("hv: hardware virtualization unavailable");
}

hv_backend_t hv_backend(void) {
    return g_backend;
}

aether_status_t hv_launch_guest(hv_guest_ctx_t *ctx) {
    if (!ctx || g_backend == HV_BACKEND_NONE) {
        return AETHER_EINVAL;
    }
    log_info("hv: guest launch requested");
    return AETHER_OK;
}

void hv_vmexit_dispatch(uint64_t reason) {
    (void)reason;
}
