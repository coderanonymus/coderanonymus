#ifndef AETHER_SECURITY_H
#define AETHER_SECURITY_H

#include <stdint.h>
#include "aether/types.h"

typedef struct {
    uint64_t bits[4];
} capability_token_t;

typedef struct {
    uint64_t allow_mask[8];
} syscall_filter_t;

void security_init(void);
aether_status_t security_set_filter(pid_t pid, const syscall_filter_t *filter);
aether_status_t security_grant_cap(pid_t pid, capability_token_t cap);
int security_check_cap(pid_t pid, uint32_t cap_id);

#endif
