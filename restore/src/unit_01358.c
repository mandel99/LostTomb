#include "actor_callback_496140.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
uint32_t FUN_004a35e0(int32_t id)
{
    uint8_t action=B(0xbcd630u+(uint32_t)id*388u);uint32_t queue;
    if(action==6)return 1;if(action!=7)return 1;
    for(queue=0xeaaf68u;queue<0xeadb28u;queue+=224u)if(U(queue)&&restore_actor_496140_queue_contains((const void *)(uintptr_t)queue,(uint32_t)id))return 1;
    return 0;
}
