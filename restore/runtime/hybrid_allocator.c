/* Only selected by RESTORE_BUILD_HYBRID=ON. The historical hybrid must keep
   allocating and freeing through the same original CRT as its native callers.
   This file is never compiled or linked into the independent C executable. */
#include "host_allocator.h"
void *restore_heap_allocate(size_t bytes)
{return ((void *(__cdecl *)(size_t))0x00561ba6u)(bytes);}
void restore_heap_free(void *allocation)
{((void (__cdecl *)(void *))0x00561bdcu)(allocation);}
void *restore_heap_calloc(size_t count,size_t width)
{return ((void *(__cdecl *)(size_t,size_t))0x00563129u)(count,width);}
void *restore_heap_reallocate(void *allocation,size_t bytes)
{return ((void *(__cdecl *)(void *,size_t))0x005645a0u)(allocation,bytes);}
size_t restore_heap_size(const void *allocation)
{return ((size_t (__cdecl *)(const void *))0x005647c2u)(allocation);}
