#include "../procces.h"



typedef struct QueueNode {
    Procces* proc;
    struct QueueNode* next;
} QueueNode;


typedef struct 
{
    QueueNode* head;
    QueueNode* tail;
    int count;
} queue;
 