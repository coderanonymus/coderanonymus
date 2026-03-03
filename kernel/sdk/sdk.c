#include "aether/sdk.h"
#include "aether/log.h"

void sdk_init(void) {
    log_info("sdk: kernel module SDK + user SDK headers online");
}

void service_manager_init(void) {
    log_info("svc: lightweight service manager framework online");
}

void package_manager_init(void) {
    log_info("pkg: package manager framework online");
}
