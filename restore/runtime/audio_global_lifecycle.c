#include "audio_global_lifecycle.h"
#include "sound_file_lookup.h"
#include <stdlib.h>
extern void FUN_0051f780(void);
void restore_sound_global_construct(void){(void)FUN_0055e38b(RESTORE_SOUND);}
void restore_music_global_construct(void){(void)FUN_00539550(RESTORE_MUSIC);}
void restore_sound_global_cleanup(void){FUN_0055e58a(RESTORE_SOUND);}
void restore_music_global_cleanup(void){FUN_0051f780();}
void restore_sound_global_register_cleanup(void){(void)atexit(FUN_004cdfa0);}
void restore_music_global_register_cleanup(void){(void)atexit(FUN_004cdfe0);}
void restore_sound_global_initialize(void){FUN_004cdf80();FUN_004cdf90();}
void restore_music_global_initialize(void){FUN_004cdfc0();FUN_004cdfd0();}
void FUN_004cdf70(void){restore_sound_global_initialize();}
void FUN_004cdfa0(void){restore_sound_global_cleanup();}
void FUN_004cdfb0(void){restore_music_global_initialize();}
void FUN_004cdfe0(void){restore_music_global_cleanup();}
