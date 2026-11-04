#include "sound_stopping.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
uint32_t restore_sound_stop_sample(RestoreSound *self,int32_t channel)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    if(!U(base+0x200u))(void)_AIL_end_sample_4(U(U(base+0x19cu)+(uint32_t)channel*4u));
    return 1;
}
uint32_t restore_sound_stop_3d_sample(RestoreSound *self,int32_t channel)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    if(!U(base+0x200u))(void)_AIL_end_3D_sample_4(U(U(base+0x1f0u)+(uint32_t)channel*4u));
    return 1;
}
void restore_sound_stop_all_samples(RestoreSound *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,i;
    if(U(base+0x200u))return;
    /* Each member performs its own fresh disabled test, even after callbacks. */
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i)(void)FUN_0055eb99(self,(int32_t)i);
    for(i=0;(int32_t)i<(int32_t)U(base+0x1e4u);++i)(void)FUN_0055ebd4(self,(int32_t)i);
}
int32_t restore_sound_find_channel_file(RestoreSound *self,int32_t file)
{
    uint32_t base=(uint32_t)(uintptr_t)self,i;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i)
        if(U(U(base+0x1a8u)+i*40u+4u)==(uint32_t)file)return (int32_t)i;
    return -1;
}
