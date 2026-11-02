#include "sprite_shutdown.h"
#include "host_allocator.h"
#include "sprite_auxiliary.h"
#include <stdio.h>
#include <stdlib.h>
void restore_sprite_shutdown_free(void *allocation){restore_heap_free(allocation);}
int restore_sprite_shutdown_register(void (*callback)(void)){return atexit(callback);}
void restore_sprite_shutdown_delete(void *object,uint32_t flags)
{
 fprintf(stderr,"Unsupported sprite-shutdown virtual destructor for object %p, flags %lu.\n",object,(unsigned long)flags);abort();
}
void restore_sprite_shutdown_slot(int *slot)
{
 FUN_0055d960(slot);
}
