
#include "file.h"




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
