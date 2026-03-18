#ifndef SHM_H
#define SHM_H

#include <stdint.h>
#include "../pcb/procces.h"

#define MAX_SHM 8
#define SHM_BLOCK_SIZE 4096

typedef struct {
    uint32_t id;
    uint32_t key;
    uint8_t* addr;
    uint32_t size;
    uint32_t refcount;
    Segment* Segment;
} shm_t;

extern shm_t shm_table[MAX_SHM];

void shm_init(void);
int shm_create(uint32_t key, uint32_t size);
void* shm_attach(int shmid, Procces* proc);
int shm_detach(int shmid, Procces* proc);


#endif