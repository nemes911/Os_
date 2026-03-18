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
    current_pcb = pcb_create(0, MAX_PRIORITY);
    current_pcb->state = PROC_RUNNING;
    current_pcb->quantum = DEFAULT_QUANTUM;
}

Procces* pcb_create(uint32_t pid, int priority){
    if(pcb_index >= 32) return NULL;
    Procces* p = &pcb_pool[pcb_index++];
    p->pid = pid;
    p->prioryty = priority;
    p->quantum = DEFAULT_QUANTUM + (priority * 3);
    p->state = PROC_READY;
    memset(p->fds, -1, sizeof(p->fds));
    p->open_count =0;
    p->esp = 0;
    p->eip = 0;
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

int change_priority(Procces* proc, int new_priority)
{
    if (new_priority < proc->prioryty) return -1;   // ошибка
    proc->prioryty = new_priority;
    proc->quantum  = DEFAULT_QUANTUM + (new_priority * 3);  // сразу пересчитываем квант
    return 0;
}


void schedule(void){
    if(!current_pcb) return;

    Procces* prev = current_pcb;

    if(current_pcb->state == PROC_RUNNING){
        current_pcb->state = PROC_READY;
        current_pcb->next = NULL;

        if(!ready_queque){
            ready_queque = current_pcb;
        } else {
            Procces* tmp = ready_queque;
            while(tmp->next) tmp = tmp->next;
            tmp->next = current_pcb;
        }
        //enqueue(current_pcb, ready_queque);
    }

    Procces* best     = NULL;
    Procces* best_prev = NULL;
    Procces* tmp      = ready_queque;
    Procces* prev     = NULL;
    int max_prio      = -9999;

    while (tmp)
    {
        if(tmp->prioryty > max_prio){
            max_prio = tmp->prioryty;
            best = tmp;
            best_prev = prev;
        }
        prev = tmp;
        tmp = tmp->next;
    }

    if(!best) return;

    if(best == ready_queque){
        ready_queque = best->next;
    } else if(prev){
        prev->next = best->next;
    }

    current_pcb = best;
    current_pcb->state = PROC_RUNNING;
    current_pcb->next = NULL;

    current_pcb->quantum = DEFAULT_QUANTUM + (current_pcb->prioryty * 3);
    
    /*
    current_pcb = dequeue(ready_queque);

    current_pcb->state = PROC_RUNNING;

    context_switch(&prev->esp, &current_pcb->esp);
    */
}