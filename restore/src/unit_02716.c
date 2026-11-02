#include "sound_3d.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
int32_t restore_sound_cached_3d_file(RestoreSound *sound,const int32_t *request)
{
    volatile RestoreSound *self=sound;int32_t i;
    const volatile int32_t *r=request;
    for(i=0;i<self->count3d;++i)
        if((int32_t)U(self->cache3d+(uint32_t)i*12u)==r[1]&&U(self->cache3d+(uint32_t)i*12u+4u))return i;
    return -1;
}
int32_t FUN_0055fb6f(RestoreSound *self,const int32_t *request){return restore_sound_cached_3d_file(self,request);}
