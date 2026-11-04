#include "settings_callbacks.h"
#include "media_bindings.h"
#include "sound_streams.h"
void restore_settings_sound_55f32f(void *self,int32_t volume)
{
    restore_sound_music_volume_0055f32f((RestoreSound *)self,volume);
}
void restore_settings_sound_55f3eb(void *self,int32_t volume)
{
    restore_sound_set_ambient_volume((RestoreSound *)self,volume);
}
