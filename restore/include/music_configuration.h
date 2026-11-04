#ifndef RESTORE_MUSIC_CONFIGURATION_H
#define RESTORE_MUSIC_CONFIGURATION_H
#include "music_manager.h"
void restore_music_load_configuration(RestoreMusicManager *self);
int restore_music_add_track(RestoreMusicManager *self,const char *path,int category,int32_t delay,int32_t minimum,int32_t maximum,int ambient);
void restore_music_select_population_range(RestoreMusicManager *self,int force);
void restore_music_set_range(RestoreMusicManager *self,int32_t first,int32_t last);
#endif
