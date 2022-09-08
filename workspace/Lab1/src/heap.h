#ifndef HEAP_H
#define HEAP_H

#include <inttypes.h>

void initHeap();
void* allocate(uint16_t size);
void free(void* addr);

#endif
