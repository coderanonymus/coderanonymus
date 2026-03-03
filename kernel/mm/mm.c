#include "aether/mm.h"
#include "aether/log.h"

static address_space_t kernel_aspace;
static vm_area_t static_areas[64];
static size_t next_area;

void mm_init(void) {
    kernel_aspace.pml4_phys = 0;
    kernel_aspace.areas = 0;
    next_area = 0;
    log_info("mm: init (COW + demand paging framework)");

    mm_map_region(&kernel_aspace, 0xFFFF800000000000ull, 0x200000, VM_READ | VM_WRITE);
    mm_add_guard_page(&kernel_aspace, 0xFFFF800000200000ull);
}

address_space_t *mm_kernel_aspace(void) {
    return &kernel_aspace;
}

aether_status_t mm_map_region(address_space_t *as, uint64_t start, uint64_t len, uint32_t flags) {
    if (!as || len == 0 || next_area >= (sizeof(static_areas) / sizeof(static_areas[0]))) {
        return AETHER_EINVAL;
    }

    vm_area_t *a = &static_areas[next_area++];
    a->start = start;
    a->end = start + len;
    a->flags = flags;
    a->next = as->areas;
    as->areas = a;
    return AETHER_OK;
}

aether_status_t mm_add_guard_page(address_space_t *as, uint64_t addr) {
    return mm_map_region(as, addr, 0x1000, VM_GUARD);
}

aether_status_t mm_fork_cow(address_space_t *dst, address_space_t *src) {
    if (!dst || !src) {
        return AETHER_EINVAL;
    }

    for (vm_area_t *a = src->areas; a; a = a->next) {
        uint32_t flags = a->flags;
        if (flags & VM_WRITE) {
            flags &= ~VM_WRITE;
            flags |= VM_COW;
        }
        mm_map_region(dst, a->start, a->end - a->start, flags);
    }
    return AETHER_OK;
}

aether_status_t mm_handle_page_fault(address_space_t *as, uint64_t addr, uint64_t error_code) {
    (void)error_code;
    for (vm_area_t *a = as->areas; a; a = a->next) {
        if (addr >= a->start && addr < a->end) {
            if (a->flags & VM_GUARD) {
                return AETHER_ERR;
            }
            if (a->flags & VM_DEMAND) {
                return AETHER_OK;
            }
            if (a->flags & VM_COW) {
                a->flags |= VM_WRITE;
                a->flags &= ~VM_COW;
                return AETHER_OK;
            }
            return AETHER_OK;
        }
    }
    return AETHER_ENOENT;
}
