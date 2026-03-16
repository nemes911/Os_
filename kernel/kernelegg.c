#include "Memory.h"




void kmain(void){
    
    MemoryManager mm;

    init_memory(&mm);
    init_entry();
    load_idt();
    init_timer(100);
}