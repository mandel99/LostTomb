#include "music_configuration.h"
int FUN_005395e0(RestoreMusicManager *self,const char *path,int category,int32_t delay,int32_t minimum,int32_t maximum,int ambient)
{return restore_music_add_track(self,path,category,delay,minimum,maximum,ambient);}
