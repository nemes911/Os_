#include "../kernel/mem/Memory.h"
#include "../kernel/fs/disk.h"
#include "../kernel/fs/inode.h"
#include "../kernel/fs/s5fs.h"
#include "../kernel/fs/bitmap.h"



void kmain(void){
    disk_init();
    MemoryManager  mm;
    init_memory(&mm);
    init_entry();
    load_idt();
    init_timer(100);
    inode_init();
    screen_init();
    keyboard_init();
    pcb_init();
    timer_init(100);

   

    printf("Filesystem mounted successfully\n");
}