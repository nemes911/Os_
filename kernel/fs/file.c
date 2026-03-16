
#include "file.h"


void fs_init(){
    superblock_init();

    bitmap_init();

    inode_init();

    memset(root_dir, 0, sizeof(root_dir));
}

int fs_create(const char* name){
    int ino = inode_alloc();

    if(ino < 0){
        return -1;
    }

    for(int i = 0; i < MAX_FILES; i++){
        if(root_dir[i].inode == 0){
            root_dir[i].inode = ino;

            strncpy(root_dir[i].name, name, 31);
            root_dir[i].name[31] = '\0';
            return ino;
        }
    }

    inode_free(ino);

    return -1;
}

int fs_write(uint32_t ino, void* buf, int size){
    inode_t* node = inode_get(ino);

    if(!node)
        return -1;

    int block = block_alloc();

    if(block < 0)
        return -1;
    
    node->first_block = block;
    node->size = size;
    //write_disk();

    return size;
}

int fs_read(uint32_t ino, void* buf, int size){
    inode_t* node = inode_get(ino);

    if(!node)
        return -1;
    
    if(size > node->size)
        size = node->size;

    //read_block(node->first_block, buf)

    return size;
}


int fs_delete(uint32_t ino){

    inode_t* node = inode_get(ino);

    if(!node)
        return -1;
    
    if(node->first_block)
        block_free(node->first_block);
    
    inode_free(ino);

    for(int i = 0; i < MAX_FILES; i++){
        if(root_dir[i].inode == ino){
            root_dir[i].inode = 0;
            root_dir[i].name[0] = 0;
        }
    }
    return 0;
}
