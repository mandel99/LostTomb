#include "sound_3d.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_sound_count_3d_cache_users(RestoreSound *sound)
{
    volatile RestoreSound *self=sound;int32_t i;
    for(i=0;i<self->count3d;++i)U(self->cache3d+(uint32_t)i*12u+8u)=0;
    for(i=0;i<self->count3d;++i){
        if(_AIL_3D_sample_status_4(U(self->handles3d+(uint32_t)i*4u))==4){
            uint32_t index=U(self->samples3d+(uint32_t)i*40u+24u);
            uint32_t value=U(self->cache3d+index*12u+8u)+1u;
            index=U(self->samples3d+(uint32_t)i*40u+24u);
            U(self->cache3d+index*12u+8u)=value;
        }
    }
}
void FUN_0055fbd8(RestoreSound *self){restore_sound_count_3d_cache_users(self);}
