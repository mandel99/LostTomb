#include "sound_playback.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_sound_reset_sample(RestoreSound *self,int32_t channel)
{
    static const uint32_t defaults[10]={UINT32_MAX,UINT32_MAX,100,64,1,0,UINT32_MAX,0,0,0};
    uint32_t base=(uint32_t)(uintptr_t)self,row=(uint32_t)channel*40u,i;
    /* The native body reloads samples for each store, including alias cases. */
    for(i=0;i<10u;++i)U(U(base+0x1a8u)+row+i*4u)=defaults[i];
}
