#include "aether/process.h"
#include "aether/log.h"

static process_t procs[32];
static int proc_count;

void process_init(void) {
    proc_count = 1;
    procs[0].pid = 1;
    procs[0].ppid = 0;
    procs[0].image_name = "kernel-init";
    log_info("proc: init");
}

pid_t sys_fork(void) {
    if (proc_count >= (int)(sizeof(procs)/sizeof(procs[0]))) {
        return AETHER_ENOMEM;
    }
    process_t *parent = &procs[0];
    process_t *child = &procs[proc_count++];
    *child = *parent;
    child->pid = proc_count;
    static address_space_t child_as;
    child->aspace = &child_as;
    if (parent->aspace) {
        mm_fork_cow(child->aspace, parent->aspace);
    }
    return child->pid;
}

aether_status_t sys_exec(const char *path) {
    procs[0].image_name = path;
    return AETHER_OK;
}

pid_t sys_wait(int *status) {
    if (status) *status = 0;
    return 0;
}

aether_status_t sys_kill(pid_t pid, signal_t sig) {
    (void)sig;
    for (int i = 0; i < proc_count; i++) {
        if (procs[i].pid == pid) {
            procs[i].pending_signals |= (1u << SIG_TERM);
            return AETHER_OK;
        }
    }
    return AETHER_ENOENT;
}

void process_deliver_signals(process_t *proc) {
    if (!proc) return;
    if (proc->pending_signals & (1u << SIG_TERM)) {
        proc->exit_code = 143;
    }
    proc->pending_signals = 0;
}
