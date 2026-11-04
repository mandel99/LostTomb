#ifndef RESTORE_AUDIO_GLOBAL_LIFECYCLE_H
#define RESTORE_AUDIO_GLOBAL_LIFECYCLE_H
#include "sound_initialization.h"
#include "music_manager.h"
RestoreSound *restore_sound_construct(RestoreSound *self);
RestoreMusicManager *restore_music_construct(RestoreMusicManager *self);
void restore_sound_global_initialize(void);
void restore_sound_global_construct(void);
void restore_sound_global_register_cleanup(void);
void restore_sound_global_cleanup(void);
void restore_music_global_initialize(void);
void restore_music_global_construct(void);
void restore_music_global_register_cleanup(void);
void restore_music_global_cleanup(void);
void FUN_004cdf70(void);
void FUN_004cdf80(void);
void FUN_004cdf90(void);
void FUN_004cdfa0(void);
void FUN_004cdfb0(void);
void FUN_004cdfc0(void);
void FUN_004cdfd0(void);
void FUN_004cdfe0(void);
#endif
