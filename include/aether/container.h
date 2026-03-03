#ifndef AETHER_CONTAINER_H
#define AETHER_CONTAINER_H

#include <stdint.h>
#include "aether/types.h"

typedef struct {
    uint32_t pid_ns;
    uint32_t net_ns;
    uint32_t mnt_ns;
    uint64_t mem_limit_bytes;
    uint64_t cpu_quota_us;
} container_spec_t;

void container_init(void);
aether_status_t container_launch(const container_spec_t *spec, const char *init_path);

#endif
