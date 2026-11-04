#include "sound_playback.h"
int FUN_0055edd0(uintptr_t self,const int32_t *request,int channel)
{return restore_sound_load_sample_cache((RestoreSound *)self,(const RestoreSoundRequest *)request,channel);}
