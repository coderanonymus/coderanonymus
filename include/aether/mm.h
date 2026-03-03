#ifndef AETHER_MM_H
#define AETHER_MM_H

#include <stdint.h>
#include <stddef.h>
#include "aether/types.h"

enum {
    VM_READ  = 1u << 0,
    VM_WRITE = 1u << 1,
    VM_EXEC  = 1u << 2,
    VM_USER  = 1u << 3,
    VM_COW   = 1u << 4,
    VM_GUARD = 1u << 5,
    VM_DEMAND= 1u << 6,
};

typedef struct vm_area {
    uint64_t start;
    uint64_t end;
    uint32_t flags;
    struct vm_area *next;
} vm_area_t;

typedef struct address_space {
    uint64_t pml4_phys;
    vm_area_t *areas;
} address_space_t;

void mm_init(void);
address_space_t *mm_kernel_aspace(void);
aether_status_t mm_map_region(address_space_t *as, uint64_t start, uint64_t len, uint32_t flags);
aether_status_t mm_add_guard_page(address_space_t *as, uint64_t addr);
aether_status_t mm_handle_page_fault(address_space_t *as, uint64_t addr, uint64_t error_code);
aether_status_t mm_fork_cow(address_space_t *dst, address_space_t *src);

#endif
