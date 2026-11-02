#include "actor_walking.h"
uint32_t restore_actor_walking_queue_full(const void *self)
{
    uint32_t count=restore_actor_walking_queue_count(self);
    return count==AW_U((uint32_t)(uintptr_t)self+12u)-1u;
}
