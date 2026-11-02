#include "sound_3d.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole560199. The DLL may replace handles or count during either call. */
void restore_sound_update_3d(RestoreSound *sound)
{
    volatile RestoreSound *self=sound;int32_t i;
    if(self->disabled)return;
    for(i=0;i<self->count3d;++i)
        if(_AIL_3D_sample_status_4(U(self->handles3d+(uint32_t)i*4u))==4)
            (void)_AIL_3D_update_position_8(U(self->handles3d+(uint32_t)i*4u),0);
}
void FUN_00560199(RestoreSound *self){restore_sound_update_3d(self);}
