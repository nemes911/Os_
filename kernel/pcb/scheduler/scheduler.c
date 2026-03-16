#include "../queue/queue.h"
#include "../procces.h"
#include "../queue/queue.c"
#include "../timer/time.h"
#include <stdio.h>
#include <uv.h>


#define QUANT 100

uv_timer_t timer;
queue ready_queue;



void quantum_cb(uv_timer_t* handle)
{
    current_pcb = dequeue(&ready_queue);

    if(!current_pcb)
    {
        return;//если очередь пуста
    }

    if(current_pcb->is_system == 1)
    {
        current_pcb->time_run += QUANT;
        enqueue(current_pcb, &ready_queue);
    }

    current_pcb->time_run -= QUANT;
    if(current_pcb->time_run <= 0)
    {
        current_pcb->stack_sigment->status_code = 0;
        current_pcb->code_segment->status_code = 0;
        current_pcb->data_sigment->status_code = 0;
    }

    if(current_pcb->code_segment && current_pcb->code_segment <= 0)
    {
        current_pcb->state = PROC_TERMINATED;
    }
    else
    {
        enqueue(current_pcb,& ready_queue);
    }

}