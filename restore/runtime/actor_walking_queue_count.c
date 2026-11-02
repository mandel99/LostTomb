#include "actor_walking.h"
uint32_t restore_actor_walking_queue_count(const void *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,tail=AW_U(base+8u),head=AW_U(base+4u),difference=tail-head;
    if((int32_t)difference<0)difference=AW_U(base+12u)+difference;
    return difference;
}
