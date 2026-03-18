#ifndef DISK_H
#define DISK_H

#include <stdint.h>

#define SECTOR_SIZE 512
#define DISK_SECTORS 8192
#define DISK_SIZE (DISK_SECTORS * SECTOR_SIZE)


void disk_init(void);

int disk_read_sector(uint32_t lba, void* buffer);
int disk_write_sector(uint32_t lba, const void* buffer);

#endif