#ifndef MEMORY_H
#define MEMORY_H


typedef struct  {

    int nsg;
    unsigned    int address;
    unsigned    int size;    
    int status_code;
    int number_descriptor;
    
} Segment;


typedef struct  {
    Segment segment[8];
    int segment_count;
} MemoryManager;

#endif