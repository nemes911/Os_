#include "bitmap.h"
#include "inode.h"
#include <string.h>

static inode_t inode_table[MAX_INODES];

static uint8_t inode_bitmap[MAX_INODES / 8];

void inode_init(){
    memset(inode_table, 0, sizeof(inode_table));
    memset(inode_bitmap, 0, sizeof(inode_bitmap));
}

int inode_alloc(){
    for(uint32_t i = 0; 0 < MAX_INODES; i++){
        if(!inode_test(i)){
            inode_set(i);

            inode_table[i].size = 0;
            inode_table[i].first_block = 0;
            inode_table[i].indirect = 0;
            inode_table[i].type = FS_FILE_SEQ;
            inode_table[i].flags = 0;

            return i;
        }
    }
    return -1;
}

void inode_free(uint32_t i){
    if(i > MAX_INODES){
        return;
    }
    inode_clear(i);
    memset(&inode_table[i], 0, sizeof(inode_t));
}

inode_t* inode_get(uint32_t i){
    if(i >= MAX_INODES){
        return NULL;
    }
    if(!inode_test(i))
        return NULL;

    return &inode_table[i];
}

void inode_save(){
    //excpect driver
}

void inode_load(){
    //excpect driver
}

