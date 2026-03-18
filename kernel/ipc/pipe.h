#ifndef PIPE_H
#define PIPE_H

#include <stdint.h>
#include "../pcb/procces.h"

#define MAX_PIPES 8
#define PIPE_BUFFER_SIZE 1024

typedef struct {
    uint32_t id;
    uint8_t buffer[PIPE_BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t refcount;
} pipe_t;

extern pipe_t pipe_table[MAX_PIPES];

void pipe_init();
int pipe_create(void);
int pipe_write(int pipeid, const void* data, uint32_t len);
int pipe_read(int pipeid, void* data, uint32_t len);

#endif