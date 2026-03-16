

#include "supperblock.h"

superblock_t superblock;

void superblock_init(){
    superblock.magic = S5FS_MAGIC;

    superblock.block_size = BLOCK_SIZE;

    superblock.total_blocks = 102400;
    superblock.free_blocks = 102000;

    superblock.total_inodes = 16384;
    superblock.free_inodes = 16384;

    superblock.bitmap_start = 2;
    superblock.inode_table_start = 10;
    superblock.data_start = 100;
}

void superblock_load(){
    if(superblock.magic != S5FS_MAGIC){
        superblock_init();
    }
}