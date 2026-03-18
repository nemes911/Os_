#include "pipe.h"
#include <string.h>

static pipe_t pipe_table[MAX_PIPES];

void pipe_init(void){
    memset(pipe_table, 0, sizeof(pipe_table));

}

int pipe_create(void){
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipe_table[i].refcount == 0){
            pipe_table[i].id = i;
            pipe_table[i].head = 0;
            pipe_table[i].tail = 0;
            pipe_table[i].refcount = 1;
            return i;
        }
    }
    return -1;
}

int pipe_write(int pipeid, const void* data, uint32_t len){
    if(pipeid < 0 || pipeid > MAX_PIPES || len == 0) return -1;
    pipe_t* p = &pipe_table[pipeid];

    uint32_t free_space = (PIPE_BUFFER_SIZE - (p->head - p->tail)) % PIPE_BUFFER_SIZE;
    if(len > free_space) len = free_space;

    for(uint32_t i = 0; i < len; i++){
        p->buffer[p->head % PIPE_BUFFER_SIZE] = ((uint8_t*)data)[i];
        p->head++;
    }
    return len;
}

int pipe_read(int pipeid, const void* data, uint32_t len){
    if(pipeid < 0 || pipeid > MAX_PIPES || len == 0) return -1;
    pipe_t* p = &pipe_table[pipeid];

    uint32_t avail = (p->head - p->tail) % PIPE_BUFFER_SIZE;
    if(len > avail) len = avail;

    for(uint32_t i = 0; i < len; i++){
        ((uint8_t*)data)[i] = p->buffer[p->tail % PIPE_BUFFER_SIZE];
        p->tail++;
    }
    return len;
}