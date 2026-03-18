#include "s5fs.h"
#include <stdint.h>

#define MAX_BLOCKS 102400

static uint8_t block_bitmap[MAX_BLOCKS / 8];

void bitmap_init(){
    for(uint32_t i = 0; i < sizeof(block_bitmap); i++){
        memset(block_bitmap, 0, sizeof(block_bitmap));
    }
}

static int bitmap_test(uint32_t block){
    uint32_t byte = block / 8;
    uint32_t bit = block % 8;

    return (block_bitmap[byte] & (1 << bit)) != 0;
}

static void bitmap_set(uint32_t  block){
    uint32_t byte = block / 8;
    uint32_t bit = block % 8;

    block_bitmap[byte] |= (1 << bit);
}

static void bitmap_clear(uint32_t block){
    uint32_t byte = block / 8;
    uint32_t bit = block % 8;

    block_bitmap[byte] &= ~(1 << bit);
}

int block_alloc(){
    for(uint32_t i = 0; i < MAX_BLOCKS; i++){
        if(!bitmap_test(i)){
            bitmap_set(i);
            return i;
        }
    }
    return -1;
}

void block_free(uint32_t block){
    if(block >= MAX_BLOCKS){
        return;
    }
    bitmap_clear(block);
}

void bitmap_flush(void) {
    disk_write_sector(superblock.bitmap_start, block_bitmap);
}