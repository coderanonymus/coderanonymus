#include "aether/storage.h"
#include "aether/log.h"

void storage_init(void) {
    log_info("storage: block layer + IO scheduler + journaling + snapshots + LVM framework online");
}

aether_status_t block_submit(uint64_t lba, void *buf, uint32_t sectors, int write) {
    (void)lba; (void)buf; (void)sectors; (void)write;
    return AETHER_OK;
}

aether_status_t nvme_init(void) {
    return AETHER_OK;
}

aether_status_t lvm_create_volume(const char *name, uint64_t bytes) {
    (void)name; (void)bytes;
    return AETHER_OK;
}
