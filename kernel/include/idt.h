#include <stdint.h>

#pragma pack(push, 1)


struct idtentry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
};

struct idtpointer 
{
    uint16_t limit;
    uint32_t base;
} ;

#pragma pack(pop)
#define IDT_SIZE 256 
struct idtentry idt[IDT_SIZE];
struct idtpointer idt_ptr;


