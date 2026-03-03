#ifndef AETHER_IPC_H
#define AETHER_IPC_H

#include <stddef.h>
#include "aether/types.h"

typedef struct ipc_message {
    pid_t from;
    size_t len;
    char data[64];
} ipc_message_t;

aether_status_t ipc_send(pid_t to, const void *buf, size_t len);
aether_status_t ipc_recv(pid_t self, ipc_message_t *out);

#endif
