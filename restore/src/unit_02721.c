#include "sound_3d.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole560113. Coordinates are raw float bits; do not convert integers. */
void restore_sound_position_3d(RestoreSound *sound,int32_t channel,uint32_t x,uint32_t y,uint32_t z)
{
    volatile RestoreSound *self=sound;
    if(!self->disabled)
        (void)_AIL_set_3D_position_16(U(self->handles3d+(uint32_t)channel*4u),x,y,z);
}
void FUN_00560113(RestoreSound *self,int32_t channel,uint32_t x,uint32_t y,uint32_t z)
{restore_sound_position_3d(self,channel,x,y,z);}
