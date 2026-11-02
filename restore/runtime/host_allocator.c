#include "host_allocator.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
static size_t aligned_size(size_t bytes)
{
    if(bytes>0xffffffe0u) {errno=ENOMEM;return 0;}
    if(!bytes)bytes=1;
    return (bytes+15u)&~(size_t)15u;
}
void *restore_heap_allocate(size_t bytes)
{size_t rounded=aligned_size(bytes);return rounded?malloc(rounded):NULL;}
void restore_heap_free(void *allocation) {free(allocation);}
void *restore_heap_calloc(size_t count,size_t width)
{
    size_t rounded;
    if(width&&count>SIZE_MAX/width) {errno=ENOMEM;return NULL;}
    rounded=aligned_size(count*width);
    return rounded?calloc(1,rounded):NULL;
}
void *restore_heap_reallocate(void *allocation,size_t bytes)
{
    size_t rounded;
    if(!allocation)return restore_heap_allocate(bytes);
    if(!bytes){restore_heap_free(allocation);return NULL;}
    rounded=aligned_size(bytes);
    return rounded?realloc(allocation,rounded):NULL;
}
size_t restore_heap_size(const void *allocation)
{if(!allocation){errno=EINVAL;return SIZE_MAX;}return _msize((void *)allocation);}
