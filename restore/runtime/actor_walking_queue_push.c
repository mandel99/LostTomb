#include "actor_walking.h"
uint32_t restore_actor_walking_queue_push(void *self,const int32_t *value)
{
    uint32_t base=(uint32_t)(uintptr_t)self,tail=AW_U(base+8u),capacity=AW_U(base+12u),next=tail+1u,item;
    if((int32_t)next>=(int32_t)capacity)next=0;
    if(next==AW_U(base+4u))return 0;
    item=*(const volatile uint32_t *)value;
    AW_U(base+tail*4u+20u)=item;
    AW_U(base+8u)=next;
    return 1;
}
