#include "sound_3d.h"
#include "media_bindings.h"
/* Whole55FEC8: publish first; the DLL receives the mutable argument copy. */
void restore_sound_set_eax_environment(RestoreSound *sound,uint32_t environment)
{
    volatile RestoreSound *self=sound;
    self->eax_environment=environment;
    if(!self->disabled&&self->device3d&&self->eax_supported)
        (void)_AIL_set_3D_provider_preference_12(self->device3d,0x6047e8u,(uintptr_t)&environment);
}
void FUN_0055fec8(RestoreSound *self,uint32_t environment){restore_sound_set_eax_environment(self,environment);}
