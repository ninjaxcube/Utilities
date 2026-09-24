#ifndef ALLOCATORS_H
#define ALLOCATORS_H
#include <stdint.h>

typedef struct Arena
{
    uint8_t * memory;
    uintptr_t size;
    uintptr_t offset;
} Arena_t;

typedef struct Slab
{
    uint8_t * memory;
    uintptr_t size;
    uintptr_t offset;
    uintptr_t slab_size;
    void * free_list;
} Slab_t;



#endif