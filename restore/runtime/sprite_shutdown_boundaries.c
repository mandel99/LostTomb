#include "sprite_shutdown.h"
#include "host_allocator.h"
#include <stdio.h>
#include <stdlib.h>
extern void FUN_0055d960(int *slot);
void restore_sprite_shutdown_free(void *allocation){restore_heap_free(allocation);}
int restore_sprite_shutdown_register(void (*callback)(void)){return atexit(callback);}
void restore_sprite_shutdown_delete(void *object,uint32_t flags)
{
 fprintf(stderr,"Unsupported sprite-shutdown virtual destructor for object %p, flags %lu.\n",object,(unsigned long)flags);abort();
}
void restore_sprite_shutdown_slot(int *slot)
{
 if(*slot){fprintf(stderr,"Unsupported sprite-shutdown auxiliary object %08lx (55d8b0 receiver not reconstructed).\n",(unsigned long)(uint32_t)*slot);abort();}
 FUN_0055d960(slot);
}
