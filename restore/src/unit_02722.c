#include "sound_3d.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole560150, direction then up vector, preserving all six raw DWORDs. */
void restore_sound_orientation_3d(RestoreSound *sound,int32_t channel,uint32_t dx,uint32_t dy,uint32_t dz,uint32_t ux,uint32_t uy,uint32_t uz)
{
    volatile RestoreSound *self=sound;
    if(!self->disabled)
        (void)_AIL_set_3D_orientation_28(U(self->handles3d+(uint32_t)channel*4u),dx,dy,dz,ux,uy,uz);
}
void FUN_00560150(RestoreSound *self,int32_t channel,uint32_t dx,uint32_t dy,uint32_t dz,uint32_t ux,uint32_t uy,uint32_t uz)
{restore_sound_orientation_3d(self,channel,dx,dy,dz,ux,uy,uz);}
