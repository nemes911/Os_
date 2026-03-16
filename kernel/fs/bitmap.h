#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

void bitmap_init();

int block_alloc();

void block_free(uint32_t block);
#endif