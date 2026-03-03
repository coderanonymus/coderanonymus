#include "aether/sched2.h"
#include "aether/log.h"

typedef struct {
    uint64_t vruntime;
    int cpu_affinity;
    int rt_class;
    uint64_t deadline_ns;
} sched_entity_t;

static sched_entity_t entities[64];
static int count;

void sched2_init(void) {
    count = 0;
    for (int i = 0; i < 64; i++) {
        entities[i].vruntime = 0;
        entities[i].cpu_affinity = -1;
        entities[i].rt_class = 0;
        entities[i].deadline_ns = 0;
    }
    log_info("sched2: CFS-style engine + RT/deadline classes scaffolded");
}

void sched2_tick(uint64_t now_ns) {
    for (int i = 0; i < count; i++) {
        entities[i].vruntime += 1000000;
        if (entities[i].deadline_ns && now_ns > entities[i].deadline_ns) {
            entities[i].rt_class = 1;
        }
    }
}
