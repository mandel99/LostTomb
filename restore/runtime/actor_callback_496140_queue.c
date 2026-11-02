#include "actor_callback_496140.h"
#include "actor_ring_queue.h"
uint32_t restore_actor_496140_queue_contains(const void *self,uint32_t id)
{
    return restore_actor_ring_contains(self,id);
}
