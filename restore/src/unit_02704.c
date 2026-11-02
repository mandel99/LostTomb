#include "sound_streams.h"

/* Normalize native ECX+self, one stack argument/RET4 into explicit C.
   The complete30-instruction implementation is shared with figure voice
   and settings; no uninitialized physical-register surrogate remains. */
void FUN_0055f32f(RestoreSound *self,int32_t volume)
{
    restore_sound_music_volume_0055f32f(self,volume);
}
