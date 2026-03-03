#include "aether/log.h"

void service_restart(const char *name) {
    (void)name;
    log_warn("service: restart requested");
}
