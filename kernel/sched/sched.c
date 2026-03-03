#include "aether/sched.h"
#include "aether/log.h"

static task_t *runq[SCHED_PRIO_MAX + 1];
static task_t *sleepq;

void sched_init(void) {
    for (int i = 0; i <= SCHED_PRIO_MAX; i++) {
        runq[i] = 0;
    }
    sleepq = 0;
    log_info("sched: priority scheduler online");
}

void sched_enqueue(task_t *task) {
    if (!task) return;
    int prio = task->priority;
    if (prio < 0) prio = 0;
    if (prio > SCHED_PRIO_MAX) prio = SCHED_PRIO_MAX;
    task->state = TASK_READY;
    task->next = runq[prio];
    runq[prio] = task;
}

void sched_sleep(task_t *task, uint64_t now_ns, uint64_t duration_ns) {
    if (!task) return;
    task->state = TASK_SLEEPING;
    task->wake_deadline_ns = now_ns + duration_ns;
    task->next = sleepq;
    sleepq = task;
}

void sched_block(task_t *task) {
    if (!task) return;
    task->state = TASK_BLOCKED;
}

void sched_wake(task_t *task) {
    if (!task) return;
    sched_enqueue(task);
}

void sched_tick(uint64_t now_ns) {
    task_t *prev = 0;
    task_t *t = sleepq;
    while (t) {
        if (t->wake_deadline_ns <= now_ns) {
            task_t *w = t;
            if (prev) prev->next = t->next;
            else sleepq = t->next;
            t = t->next;
            sched_enqueue(w);
            continue;
        }
        prev = t;
        t = t->next;
    }
}

task_t *sched_pick_next(void) {
    for (int p = SCHED_PRIO_MAX; p >= 0; p--) {
        if (runq[p]) {
            task_t *t = runq[p];
            runq[p] = t->next;
            t->next = 0;
            t->state = TASK_RUNNING;
            return t;
        }
    }
    return 0;
}
