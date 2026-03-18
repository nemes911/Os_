#ifndef INODE_H
#define INODE_H

#define BLOCK_SIZE 512
#include "s5fs.h"



void inode_init();

void inode_save();

void inode_load();

inode_t* inode_get(uint32_t inode);
#endif