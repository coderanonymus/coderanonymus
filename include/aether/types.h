#ifndef AETHER_TYPES_H
#define AETHER_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef int32_t pid_t;
typedef int32_t tid_t;

typedef enum {
    AETHER_OK = 0,
    AETHER_ERR = -1,
    AETHER_EINVAL = -22,
    AETHER_ENOMEM = -12,
    AETHER_ENOENT = -2,
} aether_status_t;

#endif
