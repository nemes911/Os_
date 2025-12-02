#include "procces.h"
#include "../mem//Memory.h"
#include <string.h>
#include "../pcb/Tables/tablepid.c"
#include "../pcb/queue/queue.h"



#define QUANT 100

Procces* create_init_procces(MemoryManager* mm)
{
    init_memory(mm);
    
    static Procces init;

    init.pid = 1;
    init.ppid = 0;
    strcpy(init.name, "init");
    init.prioryty = MAX_PRIORITY;
    init.code_segment = &mm->segment[0];
    init.data_sigment = &mm->segment[1];
    init.stack_sigment = &mm->segment[2];
    init.state = PROC_RUNNING;

    return &init;
}


Procces* create_procces(Procces* init, tablepid* table, const char* name, int priority, MemoryManager* mm)
{
    static Procces new_init;

    static  queue new_queque;

    new_init.pid = allocate_pid(table);
    new_init.ppid = &init->pid;
    strncpy(new_init.name, name, sizeof(new_init.name)-1);
    new_init.name[sizeof(new_init.name)-1] = '\0';
    new_init.prioryty = priority;
    new_init.code_segment = allocate_segment(mm);
    new_init.data_sigment = allocate_segment(mm);
    new_init.stack_sigment = allocate_segment(mm);
    new_init.state = PROC_READY;



    return &new_init;
}

Procces* kill_procces(Procces* init, int pid,tablepid* table, MemoryManager* mm)
{
    for(int i = 2; i < MAX_PID; i++)
    {
        if(init[i].pid == pid && init[i].state != PROC_TERMINATED)
        {
            init[i].state = PROC_TERMINATED;
                free_pid(table, pid);
                if(init[i].code_segment) init[i].code_segment->status_code = 0;
                if(init[i].data_sigment) init[i].data_sigment->status_code = 0;
                 if(init[i].stack_sigment) init[i].stack_sigment->status_code = 0;

                return &init[i];
        }
    }

    return NULL;
}

Procces* change_priority(Procces* init, int new_priority)
{
    if(new_priority < init->prioryty)
    {
        return -1;
    }
    else
    {
        init->prioryty = new_priority;
    }

    return init;
}