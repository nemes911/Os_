
#ifndef PROCCES_H
#define PROCCES_H
#define MAX_PRIORITY 19
#define MIN_PRIOTITY -20
#define DEFAULT_QUANTUM 10


#include "../mem/Memory.h"
#include "stdint.h"

typedef enum {
    PROC_READY,
    PROC_RUNNING,
    PROC_WAITING,
    PROC_SLEEPING,
    PROC_TERMINATED
} ProccesState;


typedef struct  Procces{
    int pid;
    int ppid;
    char name[32];
    int prioryty;
    Segment* code_segment;
    Segment* data_sigment;
    Segment* stack_sigment;
    ProccesState state;
    int time_run;
    int is_system;
    int fds[8];
    int open_count;
    int quantum;
    uint32_t esp;
    uint32_t eip;
    struct Procces* next;
    void (*entry) (Procces* proc); // указатель на функцию "код процесса"
}   Procces;



extern Procces* current_pcb;
extern Procces* ready_queque;

void pcb_init(void);
Procces* pcb_create(uint32_t pid);
void schedule(void);

#endif