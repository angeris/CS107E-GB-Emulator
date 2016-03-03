#include "malloc.h"

extern int __bss_end__;

static unsigned char *mem_ptr = (unsigned char *)&__bss_end__;

void *malloc(unsigned long size) {
    size = roundup(size, 8);
    void *curr_mem = (void *)mem_ptr;
    mem_ptr += size;

    return curr_mem;
}
