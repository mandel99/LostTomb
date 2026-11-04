#include "sound_initialization.h"
#include "audio_global_lifecycle.h"
void restore_sound_initialize_owned(void)
{
    static int initialized;
    if(!initialized) {FUN_004cdf70();initialized=1;}
}
