#ifndef FILE_H
#define FILE_H
#include "s5fs.h"
#define MAX_FILES 256

static dir_entry_t root_dir[MAX_FILES];


void fs_init();

int fs_create(const char* name);

int fs_read(uint32_t inode, void* buf, int size);

int fs_write(uint32_t inode, void* buf, int size);

int fs_delete(uint32_t inode);


#endif