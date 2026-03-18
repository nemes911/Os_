#include "shm.h"
#include <string.h>
#include "../mem/Memory.h"

static uint8_t shm_pool[MAX_SHM  SHM_BLOCK_SIZE] __attribute__((aligned(16)));
static shm_t shm_table[MAX_SHM];

void shm_init(void){
    memset(shm_table, 0, sizeof(shm_table));
    memset(shm_pool, 0, sizeof(shm_pool));
}

int shm_create(uint32_t key, uint32_t size){
    if (size > SHM_BLOCK_SIZE) size = SHM_BLOCK_SIZE;

    for (int i = 0; i < MAX_SHM; i++) {
        if (shm_table[i].key == key && shm_table[i].refcount > 0) {
            shm_table[i].refcount++;
            return i;
        }
        if (shm_table[i].refcount == 0) {
            shm_table[i].id       = i;
            shm_table[i].key      = key;
            shm_table[i].addr     = shm_pool + i * SHM_BLOCK_SIZE;
            shm_table[i].size     = size;
            shm_table[i].refcount = 1;
            shm_table[i].Segment = NULL;        // можно позже привязать к MemoryManager
            return i;
        }
    }
    return -1;
}

void* shm_attach(int shmid, Procces* proc) {
    if (shmid < 0 || shmid >= MAX_SHM || shm_table[shmid].refcount == 0)
        return NULL;

    // Можно потом добавить в Procces->data_sigment или отдельный shared_segment
    return shm_table[shmid].addr;
}

int shm_detach(int shmid, Procces* proc) {
    if (shmid < 0 || shmid >= MAX_SHM) return -1;
    if (--shm_table[shmid].refcount == 0) {
        shm_table[shmid].key = 0;
        shm_table[shmid].Segment = NULL;
    }
    return 0;
}

