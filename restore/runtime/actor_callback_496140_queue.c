#include "actor_callback_496140.h"
uint32_t restore_actor_496140_queue_contains(const void *self,uint32_t id)
{
    uint32_t base=(uint32_t)(uintptr_t)self;int32_t index=*(int32_t *)(uintptr_t)(base+4u),end=*(int32_t *)(uintptr_t)(base+8u);
    while(index!=end){if(*(uint32_t *)(uintptr_t)(base+20u+(uint32_t)index*4u)==id)return 1;index=(int32_t)((uint32_t)index+1u);if(index>=*(int32_t *)(uintptr_t)(base+12u))index=0;}
    return 0;
}
