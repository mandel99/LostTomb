#include "music_manager.h"
#include "audio_global_lifecycle.h"
#include <windows.h>
uint32_t restore_music_milliseconds(void) {return GetTickCount();}
void restore_music_initialize_owned(void)
{
    static int initialized;
    if(!initialized) {FUN_004cdfb0();initialized=1;}
}
