#include "msg.h"
#include <string.h>

static msg_queue_t msg_queues[MAX_MSG_QUEUES];

void msg_init(void){
    memset(msg_queues, 0, sizeof(msg_queues));
}

int msg_create(uint32_t key){
    for(int i = 0; i < MAX_MSG_QUEUES; i++){
        if(msg_queues[i].id == 0){
            msg_queues[i].id = i + 1;
            msg_queues[i].head = msg_queues[i].tal = 0;
            return i;
        }
    }
    return -1;
}

int msg_send(uint32_t pid, long type, const void* data, uint32_t len){
    if(len > MAX_MSG_SIZE) len = MAX_MSG_SIZE;

    for (int i = 0; i < MAX_MSG_QUEUES; i++) {
        if (msg_queues[i].id == pid) {   // упрощённо привязываем к pid
            uint32_t next = (msg_queues[i].head + 1) % 32;
            if (next == msg_queues[i].tal) return -1; // переполнено

            msg_queues[i].queue[msg_queues[i].head].type = type;
            memcpy(msg_queues[i].queue[msg_queues[i].head].data, data, len);
            msg_queues[i].head = next;
            return 0;
        }
    }
    return -1;
}

int msg_recv(uint32_t pid, long type, void* data, uint32_t max_len) {
    for (int i = 0; i < MAX_MSG_QUEUES; i++) {
        if (msg_queues[i].id == pid) {
            if (msg_queues[i].head == msg_queues[i].tal) return -1; // пусто

            msg_t* m = &msg_queues[i].queue[msg_queues[i].tal];
            if (type != 0 && m->type != type) return -2; // не тот тип

            uint32_t copy_len = (max_len < MAX_MSG_SIZE) ? max_len : MAX_MSG_SIZE;
            memcpy(data, m->data, copy_len);
            msg_queues[i].tal = (msg_queues[i].tal + 1) % 32;
            return copy_len;
        }
    }
    return -1;
}