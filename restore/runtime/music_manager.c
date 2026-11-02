#include "music_manager.h"
#include <windows.h>
uint32_t restore_music_milliseconds(void) {return GetTickCount();}
void restore_music_initialize_owned(void)
{
    static int initialized;
    if(!initialized) {FUN_00539550(RESTORE_MUSIC);initialized=1;}
}
