#ifndef RESTORE_SOUND_STREAMS_H
#define RESTORE_SOUND_STREAMS_H
#include <stdint.h>
#include "sound_initialization.h"
uint32_t restore_sound_open_stream(RestoreSound *self,const char *filename,int32_t channel,uint32_t loops);
void restore_sound_set_stream_volume(RestoreSound *self,int32_t channel,int32_t volume);
uint32_t restore_sound_stream_is_playing(RestoreSound *self,int32_t channel);
uint32_t restore_sound_get_stream_volume(RestoreSound *self,int32_t channel);
void restore_sound_set_music_volume(RestoreSound *self,int32_t volume);
void restore_sound_set_voice_volume(RestoreSound *self,int32_t volume);
void restore_sound_set_ambient_volume(RestoreSound *self,int32_t volume);
void restore_sound_set_sample_volume(RestoreSound *self,int32_t volume);
void FUN_0055f3eb(RestoreSound *self,int32_t volume);
void FUN_0055f67e(RestoreSound *self,int32_t volume);
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
