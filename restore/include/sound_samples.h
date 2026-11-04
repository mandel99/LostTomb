#ifndef RESTORE_SOUND_SAMPLES_H
#define RESTORE_SOUND_SAMPLES_H
#include <stdint.h>
int32_t restore_sound_load_file_id(const char *filename);
void restore_sound_load_ambient_sample(int32_t bank,const char *filename,int32_t mode);
void restore_sound_initialize_ambient_samples(void);
int32_t FUN_00539c60(const char *filename);
void FUN_0053bc80(int32_t bank,const char *filename,int32_t mode);
void FUN_0053bcf0(void);
#endif
