#include "aether/security.h"
#include "aether/log.h"

static capability_token_t caps[64];
static syscall_filter_t filters[64];

void security_init(void) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 4; j++) caps[i].bits[j] = 0;
        for (int j = 0; j < 8; j++) filters[i].allow_mask[j] = ~0ull;
    }
    log_info("sec: capability model + syscall filter framework online");
}

aether_status_t security_set_filter(pid_t pid, const syscall_filter_t *filter) {
    if (pid < 0 || pid >= 64 || !filter) return AETHER_EINVAL;
    filters[pid] = *filter;
    return AETHER_OK;
}

aether_status_t security_grant_cap(pid_t pid, capability_token_t cap) {
    if (pid < 0 || pid >= 64) return AETHER_EINVAL;
    for (int i = 0; i < 4; i++) caps[pid].bits[i] |= cap.bits[i];
    return AETHER_OK;
}

int security_check_cap(pid_t pid, uint32_t cap_id) {
    if (pid < 0 || pid >= 64 || cap_id >= 256) return 0;
    uint32_t w = cap_id / 64;
    uint32_t b = cap_id % 64;
    return (caps[pid].bits[w] & (1ull << b)) != 0;
}
