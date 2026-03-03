#ifndef AETHER_LIBC_H
#define AETHER_LIBC_H

#include <stddef.h>

size_t aether_strlen(const char *s);
int aether_strcmp(const char *a, const char *b);
void *aether_memcpy(void *dst, const void *src, size_t n);

#endif
