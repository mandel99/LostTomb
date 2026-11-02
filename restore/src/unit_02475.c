#include "music_manager.h"
#include <string.h>
RestoreMusicManager *FUN_00539550(RestoreMusicManager *self)
{
    int i;memset(&self->enabled,0,9*4);
    for(i=0;i<53;++i) {
        RestoreMusicCategory *c=&self->categories[i];
        memset(c->tracks,0,sizeof(c->tracks));memset(c->ambient_after,0,sizeof(c->ambient_after));c->count=0;
    }
    return self;
}
