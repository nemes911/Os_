#include "../include/idt.h"

extern void isr_default();
extern void irq0();
extern void irq1();


void set_idt_entry_ (int n, uint32_t handler)
{
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void init_entry()
{
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    for(int i = 0; i < 31; i++)
    {
        set_idt_entry_(i, (uint32_t)isr_default);
    }

    set_idt_entry_(32, (uint32_t)irq0);

    set_idt_entry_(33, (uint32_t)irq1);

    

}