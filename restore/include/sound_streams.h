#ifndef RESTORE_SOUND_STREAMS_H
#define RESTORE_SOUND_STREAMS_H
#include <stdint.h>
#include "sound_initialization.h"
void restore_sound_close_stream(void *controller,uint32_t channel);
void FUN_0055f38d(RestoreSound *self,int32_t volume);
uint32_t FUN_0055f449(RestoreSound *self,const char *filename,int channel,uint32_t loops);
void FUN_0055f561(RestoreSound *self,int channel);
uint32_t FUN_0055f5a1(RestoreSound *self,int channel);
uint32_t FUN_0055f5f4(RestoreSound *self,int channel);
void FUN_0055f639(RestoreSound *self,int channel,int32_t volume);
void restore_sound_music_volume_0055f32f(RestoreSound *self,int32_t volume);
void FUN_0055f32f(RestoreSound *self,int32_t volume);
uint32_t FUN_00539b20(char *filename);
#endif
