#include "procces.h"
#include "../mem//Memory.h"
#include <string.h>
#include "../pcb/Tables/tablepid.c"
#include "../pcb/queue/queue.h"
#include "../pcb/queue/queue.c"
#include "../pcb/timer/time.h"

extern void context_switch(uint32_t* old_esp, uint32_t* new_esp);

Procces* current_pcb = NULL;
Procces* ready_queque = NULL; 

static Procces pcb_pool[32];
static int pcb_index = 0;



void pcb_init(void){
    current_pcb = pcb_create(0);
    current_pcb->state = PROC_RUNNING;
    current_pcb->quantum = DEFAULT_QUANTUM;
}

Procces* pcb_create(uint32_t pid){
    if(pcb_index >= 32) return NULL;
    Procces* p = &pcb_pool[pcb_index++];
    p->pid = pid;
    p->quantum = DEFAULT_QUANTUM;
    p->state = PROC_READY;
    p->next = NULL;
    return p;
}


void proc_entry (Procces* p)
{
    
}


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
    init.time_run = 0;
    init.is_system = 1;
    init.quantum = DEFAULT_QUANTUM;
    init.next = NULL;
    return &init;
}


Procces* create_procces(Procces* init, tablepid* table, const char* name, int priority, MemoryManager* mm, queue* queue)
{
    static Procces new_init;

    

    new_init.pid = allocate_pid(table);
    new_init.ppid = init->pid;
    strncpy(new_init.name, name, sizeof(new_init.name)-1);
    new_init.name[sizeof(new_init.name)-1] = '\0';
    new_init.prioryty = priority;
    new_init.code_segment = allocate_segment(mm);
    new_init.data_sigment = allocate_segment(mm);
    new_init.stack_sigment = allocate_segment(mm);
    new_init.state = PROC_READY;
    new_init.time_run = 0;
    new_init.is_system = 0;
    new_init.quantum = DEFAULT_QUANTUM;
    new_init.next = NULL;
    enqueue(&new_init,queue);

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


void schedule(void){
    Procces* prev = current_pcb;

    if(current_pcb->state == PROC_RUNNING){
        current_pcb->state = PROC_READY;
        enqueue(current_pcb, ready_queque);
    }
    
    current_pcb = dequeue(ready_queque);

    current_pcb->state = PROC_RUNNING;

    context_switch(&prev->esp, &current_pcb->esp);
}