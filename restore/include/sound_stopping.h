#ifndef RESTORE_SOUND_STOPPING_H
#define RESTORE_SOUND_STOPPING_H
#include "sound_initialization.h"
uint32_t restore_sound_stop_sample(RestoreSound *self,int32_t channel);
uint32_t restore_sound_stop_3d_sample(RestoreSound *self,int32_t channel);
uint32_t restore_sound_stop_looping_samples(RestoreSound *self);
void restore_sound_stop_all_samples(RestoreSound *self);
int32_t restore_sound_find_channel_file(RestoreSound *self,int32_t file);
uint32_t FUN_0055eb99(RestoreSound *self,int32_t channel);
uint32_t FUN_0055ebd4(RestoreSound *self,int32_t channel);
uint32_t FUN_0055ec0f(RestoreSound *self);
int32_t FUN_0055efdc(RestoreSound *self,int32_t file);
#endif
