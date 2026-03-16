#define MAX_PID 256

//Таблица id процессов и методы работы с ней

typedef struct 
{
    int pid_flags[MAX_PID];
}tablepid;


void init_pid_table(tablepid* table)
{
    for(int i = 0; i < MAX_PID; i++)
    {
        table->pid_flags[i] = 0;
    }
    table->pid_flags[0] = 1;//зарезервирован
    table->pid_flags[1] = 1;//для потока init
}

int allocate_pid(tablepid* table)
{
    for(int i = 2; i < MAX_PID; i++)
    {
        if(table->pid_flags[i] == 0)
        {
            table->pid_flags[i] = 1;
            return i;
        }
    }
    return -1;
}

void free_pid(tablepid* table, int pid)
{
    if(pid > 1 && pid < MAX_PID)
    {
        table->pid_flags[pid] = 0;
    }
}