#include "actor_walking.h"
#include "actor_ring_queue.h"
uint32_t restore_actor_walking_queue_full(const void *self)
{
    return restore_actor_ring_full(self);
}
