#include "sound_stopping.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
uint32_t restore_sound_stop_looping_samples(RestoreSound *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,i;
    if(U(base+0x200u))return 1;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i)
        if(_AIL_sample_loop_count_4(U(U(base+0x19cu)+i*4u))==0)(void)FUN_0055eb99(self,(int32_t)i);
    return 1;
}
