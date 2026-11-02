#include "actor_callbacks_human.h"
void FUN_004b3250(int32_t id)
{
    int16_t *route=(int16_t *)(uintptr_t)(0xbcd5f8u+(uint32_t)id*388u+58u);
    if(*route>0){int16_t *owner=(int16_t *)(uintptr_t)(0xd04e58u+(uint32_t)(int32_t)*route*2u);if((int32_t)*owner==id)*owner=0;*route=0;}
}
