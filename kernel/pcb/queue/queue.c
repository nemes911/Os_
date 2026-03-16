#include "../queue/queue.h"
#include "../procces.h"
#include <stdio.h>



void enqueue(Procces* init, queue* queue)  
{
   QueueNode* node = malloc(sizeof(QueueNode));
   node->proc = init;
   node->next = NULL;
   
   if(queue->tail)
   {
        queue->tail->next = node;
   }
   else
   {
        queue->head = node;
   }

   queue->tail = node;

   queue->count++;
}

Procces* dequeue(queue* queue)
{
    if(!queue->head) return NULL;

    QueueNode* node = queue->head;
    Procces* p = node->proc;

    queue->head = node->next;
    if(!queue->head)
    {
        queue->tail = NULL;
    }

    free(node);
    queue->count--;

    return p;
}

Procces* peek(queue* queue)
{
    if(!queue->head) return NULL;

    return queue->head->proc;
}

void queue_init(queue* q){
    q->head = NULL;
    q->tail = NULL;
    q->count = 0;
}