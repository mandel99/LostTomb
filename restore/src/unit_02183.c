#include "actor_ring_queue.h"
void FUN_004f64b0(void)
{
    uint32_t index;
    for(index=0;index<50u;++index)
        (void)restore_actor_ring_construct((void *)(uintptr_t)(0xeaaf68u+index*224u));
}
