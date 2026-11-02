#include "actor_walking.h"
#include "actor_ring_queue.h"
uint32_t restore_actor_walking_queue_push(void *self,const int32_t *value)
{
    return restore_actor_ring_push(self,value);
}
