#ifndef AETHER_PROCESS_H
#define AETHER_PROCESS_H

#include "aether/types.h"
#include "aether/mm.h"

typedef enum {
    SIG_TERM = 15,
    SIG_KILL = 9,
    SIG_CHLD = 17,
} signal_t;

typedef struct process {
    pid_t pid;
    pid_t ppid;
    int exit_code;
    address_space_t *aspace;
    uint32_t pending_signals;
    const char *image_name;
} process_t;

void process_init(void);
pid_t sys_fork(void);
aether_status_t sys_exec(const char *path);
pid_t sys_wait(int *status);
aether_status_t sys_kill(pid_t pid, signal_t sig);
void process_deliver_signals(process_t *proc);

#endif
