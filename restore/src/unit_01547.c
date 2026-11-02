#include "flood_simulation.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
extern int32_t FUN_004c5120(int32_t);
int32_t restore_flood_quality(uintptr_t self)
{
    uint32_t player=U(0xeaac28);
    int32_t quality=I(self+0x14);
    if (*(int16_t *)(uintptr_t)(0xea5ce8+player*0x49d8u) &&
        !FUN_004c5120(I(0xea5f9c+player*0x49d8u)))
        quality=(int32_t)((uint32_t)quality+10u);
    return quality>100?100:quality;
}
/* Native UI callers select the global load/flood settings object. */
int32_t FUN_004bd590(void) {return restore_flood_quality(0x00d38360u);}
