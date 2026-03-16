#include "Memory.h"
#include <stddef.h>


void init_memory(MemoryManager* mm)
{
    mm->segment_count = 8;

    mm->segment[0] = (Segment){0, 0x00000000, 0xFFFFFFFF, 1, 1};
    mm->segment[1] = (Segment){1, 0x00000000, 0xFFFFFFFF, 1, 2};
    mm->segment[2] = (Segment){2, 0x00000000, 0xFFFFFFFF, 1, 3};
    mm->segment[3] = (Segment){3, 0x00000000, 0xFFFFFFFF, 1, 4};
    mm->segment[4] = (Segment){4, 0x00000000, 0xFFFFFFFF, 1, 5};
    mm->segment[5] = (Segment){5, 0x00000000, 0xFFFFFFFF, 1, 6};
    mm->segment[6] = (Segment){6, 0x00000000, 0xFFFFFFFF, 1, 7};
    mm->segment[7] = (Segment){7, 0x00000000, 0xFFFFFFFF, 1, 8};
}

Segment* allocate_segment(MemoryManager* mm)
{
    for(int i = 0; i < mm->segment_count; i++)
    {
        if(mm->segment[i].status_code == 0 )
        {
            mm->segment[i].status_code = 1;

            return &mm->segment[i];
        }
    }
    return NULL;
}

int get_segment_count(MemoryManager* mm)
{
    return mm->segment_count;
}

Segment* get_segment(MemoryManager* mm, int nsg)
{
    for(int i = 0; mm->segment_count > i; i++)
    {
        if (mm->segment[i].nsg == nsg && mm->segment[i].status_code == 1 )
            return &mm->segment[i];
           
    }

    return NULL;
}

unsigned int get_segment_addres(MemoryManager* mm, int nsg)
{
    Segment* Segment = get_segment(mm, nsg);
    if(Segment != NULL)
    return Segment->address;
    else
    {
        return 0; //в дальнейшем добавить блокировку locks 
    }
}

int is_segment_active(Segment* Segment)
{
    if(Segment->status_code == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}