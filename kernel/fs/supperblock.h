#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <stdint.h>
#include "s5fs.h"

extern superblock_t superblock;

void superblock_init();

void superblock_load();

void superblock_save();

#endif