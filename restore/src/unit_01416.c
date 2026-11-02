#include "actor_callbacks_human.h"
void FUN_004b1d50(int32_t id)
{
    uint32_t a=0xbcd5f8u+(uint32_t)id*388u;int16_t *timer=(int16_t *)(uintptr_t)(a+54u);
    if(*timer<0)*timer=0;
    *timer=(int16_t)((uint16_t)*timer+1u);
    if(*timer>=128){*timer=127;*(uint8_t *)(uintptr_t)(a+14u)=2;}
}
