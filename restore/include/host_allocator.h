#ifndef RESTORE_HOST_ALLOCATOR_H
#define RESTORE_HOST_ALLOCATOR_H
#include <stddef.h>
#include <stdint.h>
/* The independent C program uses its own CRT allocator consistently. Original
   private-heap handles and small-block metadata do not belong to this process. */
void *restore_heap_allocate(size_t bytes);
void restore_heap_free(void *allocation);
void *restore_heap_calloc(size_t count,size_t width);
void *restore_heap_reallocate(void *allocation,size_t bytes);
size_t restore_heap_size(const void *allocation);
void *FUN_00561ba6(uint32_t bytes);
void FUN_00561bdc(void *allocation);
void *FUN_00563129(uint32_t count,uint32_t width);
void *FUN_005645a0(void *allocation,uint32_t bytes);
size_t FUN_005647c2(const void *allocation);
#endif
