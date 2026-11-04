#ifndef RESTORE_SOUND_BANKS_H
#define RESTORE_SOUND_BANKS_H
#include <stdint.h>
void restore_sound_load_building_sample(int32_t bank,const char *filename,int32_t mode);
void restore_sound_initialize_building_samples(void);
void restore_sound_load_ui_sample(int32_t index,const char *filename);
void restore_sound_initialize_ui_samples(void);
void restore_sound_play_ui_sample(int32_t index);
void restore_sound_load_actor_sample(int32_t bank,const char *filename,int32_t variant);
void restore_sound_initialize_actor_samples(void);
void FUN_00539cf0(int32_t bank,const char *filename,int32_t mode);
void FUN_00539d60(void);
void FUN_0053af70(int32_t index,const char *filename);
void FUN_0053af90(void);
void FUN_0053b090(int32_t index);
void FUN_0053b100(int32_t bank,const char *filename,int32_t variant);
void FUN_0053b130(void);
#endif
