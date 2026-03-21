#include <stdint.h>

uint32_t* page_directory = (uint32_t*)0x100000;
void paging_init() {
    // 4 КБ страницы, identity mapping 0-4 МБ
    for (int i = 0; i < 1024; i++)
        page_directory[i] = (i * 0x1000) | 3;  // present + rw
    asm("mov %0, %%cr3" :: "r"(page_directory));
    asm("mov %%cr0, %0" : "=r"(uint32_t cr0));
    cr0 |= 0x80000000;
    asm("mov %0, %%cr0" :: "r"(cr0));
}