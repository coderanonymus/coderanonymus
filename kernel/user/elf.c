#include "aether/elf.h"

#define R_X86_64_RELATIVE 8u

aether_status_t elf_apply_relocations(uint8_t *image, const elf_rela_t *rela, uint64_t count, uint64_t base) {
    if (!image || !rela) return AETHER_EINVAL;

    for (uint64_t i = 0; i < count; i++) {
        uint32_t type = (uint32_t)(rela[i].info & 0xffffffffu);
        if (type == R_X86_64_RELATIVE) {
            uint64_t *loc = (uint64_t *)(void *)(image + rela[i].offset);
            *loc = base + (uint64_t)rela[i].addend;
        }
    }

    return AETHER_OK;
}
