#include "sound_3d.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
int32_t restore_sound_available_3d_channel(RestoreSound *sound)
{
    volatile RestoreSound *self=sound;int32_t i;
    if(self->provider==5||self->disabled)return -1;
    for(i=0;i<self->count3d;++i)
        if(_AIL_3D_sample_status_4(U(self->handles3d+(uint32_t)i*4u))!=4)return i;
    return -1;
}
int32_t FUN_0055faf7(RestoreSound *self){return restore_sound_available_3d_channel(self);}
