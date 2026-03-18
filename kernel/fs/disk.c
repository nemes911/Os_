#include "disk.h"
#include <string.h>

static uint8_t ram_disk[DISK_SIZE];

void disk_init(void){
    memset(ram_disk, 0, DISK_SIZE);
}

// disk.c
int disk_read_sector(uint32_t lba, void* buffer){
    if(lba >= DISK_SECTORS) return -1;
    uint8_t* src = ram_disk + (lba * SECTOR_SIZE);
    memcpy(buffer, src, SECTOR_SIZE);           // ← memcpy вместо memccpy_s
    return 0;
}

int disk_write_sector(uint32_t lba, const void* buffer){
    if(lba >= DISK_SECTORS) return -1;
    uint8_t* dst = ram_disk + (lba * SECTOR_SIZE);
    memcpy(dst, buffer, SECTOR_SIZE);
    return 0;
}

