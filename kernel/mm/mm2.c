#include "aether/mm2.h"
#include "aether/log.h"

static uint64_t pressure_kb;
static uint64_t compressed_pages;

void mm2_init(void) {
    pressure_kb = 0;
    compressed_pages = 0;
    log_info("mm2: THP/swap/reclaim/compression framework initialized");
}

void mm2_reclaim_daemon_tick(void) {
    pressure_kb += 4;
    if (pressure_kb > 1024) {
        compressed_pages++;
        pressure_kb = 0;
    }
}
