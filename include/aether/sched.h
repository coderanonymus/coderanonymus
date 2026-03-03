#ifndef AETHER_SCHED_H
#define AETHER_SCHED_H

#include <stdint.h>
#include "aether/types.h"

#define SCHED_PRIO_MAX 31

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_SLEEPING,
    TASK_BLOCKED,
    TASK_ZOMBIE,
} task_state_t;

typedef struct task {
    pid_t pid;
    int priority;
    task_state_t state;
    uint64_t wake_deadline_ns;
    struct task *next;
} task_t;

void sched_init(void);
void sched_tick(uint64_t now_ns);
void sched_enqueue(task_t *task);
void sched_sleep(task_t *task, uint64_t now_ns, uint64_t duration_ns);
void sched_block(task_t *task);
void sched_wake(task_t *task);
task_t *sched_pick_next(void);

#endif
