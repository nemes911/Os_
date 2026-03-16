#ifndef S5FS_H
#define S5FS_H


#include <stdint.h>


#define MAX_BLOCKS 102400
#define MAX_INODES 16384
#define BLOCK_SIZE 4096
#define S5FS_MAGIC 0x53464653
#define FS_FILE_SEQ 1
#define FS_FILE_DIR 2
#define FS_FILE_DEV 3
#define FS_FILE_RAND 4

#pragma pack(push,1)

typedef struct superblock{
    uint32_t magic;

    uint32_t total_blocks;
    uint32_t free_blocks;

    uint32_t total_inodes;
    uint32_t free_inodes;

    uint32_t block_size;

    uint32_t bitmap_start;
    uint32_t inode_table_start;
    uint32_t data_start;
} superblock_t;


#pragma pack(pop)

#pragma pack(push,1)

typedef struct dir_entry {
    uint32_t inode;
    char name[32];
    
}dir_entry_t; 

#pragma pack(pop)

#pragma pack(push,1)

typedef struct inode{
    uint32_t size; //4
    uint32_t first_block;
    uint32_t indirect;
    uint16_t type;
    uint16_t flags;
} inode_t;

#pragma pack(pop)



void fs_init();



int inode_alloc();//+

void inode_free(uint32_t inode);//++

int block_alloc();

void block_free(uint32_t block);


#endif