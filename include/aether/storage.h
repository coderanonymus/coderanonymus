#ifndef AETHER_STORAGE_H
#define AETHER_STORAGE_H

#include <stdint.h>
#include "aether/types.h"

void storage_init(void);
aether_status_t block_submit(uint64_t lba, void *buf, uint32_t sectors, int write);
aether_status_t nvme_init(void);
aether_status_t lvm_create_volume(const char *name, uint64_t bytes);

#endif
