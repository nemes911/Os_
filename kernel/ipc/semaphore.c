typedef struct {
    int value;
    struct Process* wait_queue;
} semaphore_t;

void sem_init(semaphore_t* sem, int v) { sem->value = v; }
void sem_wait(semaphore_t* sem) {
    while (sem->value <= 0) block_current();
    sem->value--;
}
void sem_signal(semaphore_t* sem) {
    sem->value++;
    wakeup_one();
}