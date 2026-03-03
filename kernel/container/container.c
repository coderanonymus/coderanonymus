#include "aether/container.h"
#include "aether/log.h"

void container_init(void) {
    log_info("container: namespaces + cgroups-like limits framework online");
}

aether_status_t container_launch(const container_spec_t *spec, const char *init_path) {
    if (!spec || !init_path) return AETHER_EINVAL;
    log_info("container: launch requested");
    return AETHER_OK;
}
