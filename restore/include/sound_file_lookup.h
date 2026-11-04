#ifndef RESTORE_SOUND_FILE_LOOKUP_H
#define RESTORE_SOUND_FILE_LOOKUP_H
#include "sound_initialization.h"
int FUN_0055dd3f(RestoreSoundFileTable *self,const char *path,int copy);
int32_t restore_sound_register_file(RestoreSoundFileTable *self,const char *path,int32_t copy);
RestoreSoundFileTable *restore_sound_file_table_initialize(RestoreSoundFileTable *self);
void restore_sound_file_table_clear(RestoreSoundFileTable *self);
void restore_sound_file_table_destroy(RestoreSoundFileTable *self);
int restore_sound_file_table_resize(RestoreSoundFileTable *self,int32_t count);
RestoreSoundFileTable *FUN_0055dbc0(RestoreSoundFileTable *self);
void FUN_0055dbf8(RestoreSoundFileTable *self);
void FUN_0055dc18(RestoreSoundFileTable *self);
void restore_sound_file_allocation_cleanup_frame(uint32_t frame);
void FUN_0056e140(uint32_t frame);
void restore_sound_destroy(RestoreSound *self);
void FUN_0055e58a(RestoreSound *self);
void restore_sound_file_table_cleanup_frame(uint32_t frame);
void FUN_0056e168(uint32_t frame);
#endif
