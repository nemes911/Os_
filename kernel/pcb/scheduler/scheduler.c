#include "../queue/queue.h"
#include "../procces.h"
#include "../queue/queue.c"


#define QUANT 100


 fork(queue* queue)
{
    Procces* init = peek(queue);

    if(init->state != PROC_TERMINATED)
    {
        enqueue(&init, queue);
    }
}