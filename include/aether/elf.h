#ifndef AETHER_ELF_H
#define AETHER_ELF_H

#include <stdint.h>
#include "aether/types.h"

typedef struct {
    uint64_t offset;
    uint64_t info;
    int64_t addend;
} elf_rela_t;

aether_status_t elf_apply_relocations(uint8_t *image, const elf_rela_t *rela, uint64_t count, uint64_t base);

#endif
