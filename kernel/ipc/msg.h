#ifndef MSG_H
#define MSG_H

#include "../pcb/procces.h"
#include <string.h>

#define MAX_MSG_QUEUES 8
#define MAX_MSG_SIZE 64

typedef struct{
    long type;
    char data[MAX_MSG_SIZE];
} msg_t;

typedef struct {
    uint32_t id;
    msg_t queue[32];
    uint32_t head;
    uint32_t tal;
} msg_queue_t;

extern msg_queue_t msg_queues[MAX_MSG_QUEUES];

void msg_init(void);
int msg_create(uint32_t key);
int msg_send(uint32_t pid, long type, const void* data, uint32_t len);
int msg_recv(uint32_t pid, long type, const void* data, uint32_t max_len);

#endif MSG_H