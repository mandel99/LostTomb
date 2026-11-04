#ifndef RESTORE_SOUND_PLAYBACK_H
#define RESTORE_SOUND_PLAYBACK_H
#include "sound_initialization.h"
/* First five DWORDs of a 40-byte channel record. */
typedef struct RestoreSoundRequest {int32_t owner,file,volume,pan,loops;} RestoreSoundRequest;
typedef char restore_sound_request_size[(sizeof(RestoreSoundRequest)==20)?1:-1];
void restore_sound_reset_sample(RestoreSound *self,int32_t channel);
void restore_sound_refresh_cache_references(RestoreSound *self);
int32_t restore_sound_load_sample_cache(RestoreSound *self,const RestoreSoundRequest *request,int32_t channel);
int32_t restore_sound_find_available_channel(RestoreSound *self);
int32_t restore_sound_find_cached_sample(RestoreSound *self,const RestoreSoundRequest *request);
int32_t restore_sound_play_sample(RestoreSound *self,const RestoreSoundRequest *request);
#endif
