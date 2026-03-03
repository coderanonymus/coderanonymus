#include "aether/ipc.h"

#define IPC_MAX 32

static ipc_message_t q[IPC_MAX];
static size_t head;
static size_t tail;

aether_status_t ipc_send(pid_t to, const void *buf, size_t len) {
    (void)to;
    if (len > sizeof(q[0].data)) {
        len = sizeof(q[0].data);
    }
    size_t next = (tail + 1) % IPC_MAX;
    if (next == head) {
        return AETHER_ERR;
    }
    q[tail].from = 0;
    q[tail].len = len;
    const char *src = (const char *)buf;
    for (size_t i = 0; i < len; i++) q[tail].data[i] = src[i];
    tail = next;
    return AETHER_OK;
}

aether_status_t ipc_recv(pid_t self, ipc_message_t *out) {
    (void)self;
    if (!out || head == tail) {
        return AETHER_ENOENT;
    }
    *out = q[head];
    head = (head + 1) % IPC_MAX;
    return AETHER_OK;
}
