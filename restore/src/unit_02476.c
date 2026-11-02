#include "music_manager.h"
#include <string.h>
int FUN_005395e0(RestoreMusicManager *self,const char *path,int category,int32_t delay,int32_t minimum,int32_t maximum,int ambient)
{
    RestoreMusicCategory *c;RestoreMusicTrack *track;int index;size_t length;
    if(category<0||category>=53||!path)return -1;
    length=strlen(path);if(length>=sizeof(self->categories[0].tracks[0].path))return -1;
    c=&self->categories[category];index=ambient?c->ambient_count:c->count;
    if(index<0||index>=(ambient?10:50))return -1;
    track=&c->tracks[index+(ambient?50:0)];memcpy(track->path,path,length+1);
    track->delay=delay;track->minimum=ambient?0:minimum;track->maximum=ambient?0:maximum;
    if(ambient)++c->ambient_count;else ++c->count;return 1;
}
