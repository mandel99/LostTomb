#include "sound_3d.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole55FA1C. The cache-index DWORD at+24 is intentionally untouched. */
void restore_sound_reset_3d_channel(RestoreSound *sound,int32_t channel)
{
    volatile RestoreSound *self=sound;uint32_t offset=(uint32_t)channel*40u;
    U(self->samples3d+offset)=UINT32_MAX;U(self->samples3d+offset+4u)=UINT32_MAX;
    U(self->samples3d+offset+8u)=100;U(self->samples3d+offset+12u)=64;
    U(self->samples3d+offset+16u)=1;U(self->samples3d+offset+20u)=0;
    U(self->samples3d+offset+28u)=0;U(self->samples3d+offset+32u)=0;U(self->samples3d+offset+36u)=0;
}
void FUN_0055fa1c(RestoreSound *self,int32_t channel){restore_sound_reset_3d_channel(self,channel);}
