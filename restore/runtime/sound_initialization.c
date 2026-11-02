#include "sound_initialization.h"
void restore_sound_initialize_owned(void)
{
    static int initialized;
    if(!initialized) {FUN_0055e38b(RESTORE_SOUND);initialized=1;}
}
