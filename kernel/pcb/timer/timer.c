


#include "time.h"
#include "../procces.h"
#include "../procces.c"

static inline void outb(uint16_t port, uint8_t val){
   // asm __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}


uint32_t tick = 0;

void init_timer(uint32_t freequency){
    uint32_t divisior = 1193180 / freequency;
    outb(0x43, 0x36);
    outb(0x40, divisior & 0xFF);
    outb(0x40, (divisior >> 8) & 0xFF);
}

void irq0(void){
    tick++;

    if(!current_pcb){
        outb(0x20, 0x20);
        return;
    }

    current_pcb->quantum--;

    if(current_pcb->quantum <= 0){
        current_pcb->quantum = DEFAULT_QUANTUM;
        schedule();
    }

    outb(0x20, 0x20);  //EOI
}
