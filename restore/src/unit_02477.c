#include "music_manager.h"
#include <string.h>
void FUN_00539730(RestoreMusicManager *self,int force)
{
    int category=*(int8_t *)0xe38ea8u;uint32_t profile=*(uint32_t *)0xeaac28u;int32_t population;
    RestoreMusicCategory *c;RestoreMusicTrack *track;int first,last;
    if(category<0||category>=53||profile>=8||self->current<0||self->current>=60)return;
    memcpy(&population,(const void *)(uintptr_t)(0xea1894u+profile*0x49d8u),4);
    c=&self->categories[category];if(c->count<0||c->count>50)return;
    track=&c->tracks[self->current];
    if(population>track->maximum||population<track->minimum||force) {
        for(first=0;first<c->count;++first)if(c->tracks[first].minimum<=population&&population<=c->tracks[first].maximum)break;
        for(last=first;last<c->count;++last)if(c->tracks[last].minimum>population)break;
        FUN_00539a50(self,first,last-1);
    }
    if(self->current<self->first||self->current>self->last)self->current=self->first;
    if(self->first!=self->previous) {
        self->previous=self->first;if(!self->waiting)self->phase=1;
    }
}
