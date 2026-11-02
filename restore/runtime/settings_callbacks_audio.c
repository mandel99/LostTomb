#include "settings_callbacks.h"
#include "media_bindings.h"
#include "sound_streams.h"
void restore_settings_sound_55f32f(void *self,int32_t volume)
{
    restore_sound_music_volume_0055f32f((RestoreSound *)self,volume);
}
void restore_settings_sound_55f3eb(void *self,int32_t volume)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    SSET_PUT32(base+0x158u,(uint32_t)volume);
    if(SSET_U(base+0x200u)==0 && SSET_U(base+0x1e0u)!=0 && _AIL_stream_status_4(SSET_U(base+0x1e0u))==4)
        (void)_AIL_set_stream_volume_8(SSET_U(base+0x1e0u),(uint32_t)volume);
}
