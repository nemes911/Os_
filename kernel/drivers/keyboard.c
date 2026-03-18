#include "keyboard.h"
#include "../pcb/timer/time.h"

static char keyboard_buffer[256];
static uint8_t head = 0; tail = 0;

static const char scancode_to_ascii[] = {
    0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' '
};

static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void keyboard_init(void){
    head = tail = 0;
}

void irq1(void){
    uint8_t scancode = inb(0x60);

    if(scancode < 128){
        char c = scancode_to_ascii[scancode];
        if(c){
            keyboard_buffer[head] = c;
            head = (head + 1) % 256;
        }
    }
    outb(0x20, 0x20);
}

char getchar(void){
    if(head == tail) return 0;
    char c = keyboard_buffer[tail];
    tail = (tail + 1) % 256;
    return c;
}