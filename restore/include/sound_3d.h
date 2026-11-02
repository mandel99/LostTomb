#ifndef RESTORE_SOUND_3D_H
#define RESTORE_SOUND_3D_H
#include "sound_initialization.h"
/* Exact556-byte private scratch of55F697. Candidates absent from enumeration
   and an unwritten attribute result retain their incoming stack bytes. */
typedef struct RestoreSoundProviderScratch {
    uint32_t receiver,iterator,name;
    int32_t environment;
    uint32_t unused;
    int32_t index;
    uint32_t enumerated,found;
    char names[5][100];
    int32_t open_result;
    uint32_t candidates[5];
} RestoreSoundProviderScratch;
typedef char restore_provider_scratch_size[sizeof(RestoreSoundProviderScratch)==556?1:-1];
typedef char restore_provider_candidates_offset[offsetof(RestoreSoundProviderScratch,candidates)==536?1:-1];
void restore_sound_select_3d_body(RestoreSound *self,RestoreSoundProviderScratch *scratch);
void restore_sound_reset_3d_channel(RestoreSound *self,int32_t channel);
int32_t restore_sound_available_3d_channel(RestoreSound *self);
int32_t restore_sound_cached_3d_file(RestoreSound *self,const int32_t *request);
void restore_sound_count_3d_cache_users(RestoreSound *self);
int32_t restore_sound_load_3d_file(RestoreSound *self,const int32_t *request,int32_t channel);
void restore_sound_set_eax_environment(RestoreSound *self,uint32_t environment);
int32_t restore_sound_play_3d(RestoreSound *self,const int32_t *request);
void restore_sound_position_3d(RestoreSound *self,int32_t channel,uint32_t x,uint32_t y,uint32_t z);
void restore_sound_orientation_3d(RestoreSound *self,int32_t channel,uint32_t dx,uint32_t dy,uint32_t dz,uint32_t ux,uint32_t uy,uint32_t uz);
void restore_sound_update_3d(RestoreSound *self);
void FUN_0055fa1c(RestoreSound *self,int32_t channel);
int32_t FUN_0055faf7(RestoreSound *self);
int32_t FUN_0055fb6f(RestoreSound *self,const int32_t *request);
void FUN_0055fbd8(RestoreSound *self);
int32_t FUN_0055fca4(RestoreSound *self,const int32_t *request,int32_t channel);
void FUN_0055fec8(RestoreSound *self,uint32_t environment);
int32_t FUN_0055ff20(RestoreSound *self,const int32_t *request);
void FUN_00560113(RestoreSound *self,int32_t channel,uint32_t x,uint32_t y,uint32_t z);
void FUN_00560150(RestoreSound *self,int32_t channel,uint32_t dx,uint32_t dy,uint32_t dz,uint32_t ux,uint32_t uy,uint32_t uz);
void FUN_00560199(RestoreSound *self);
#endif
