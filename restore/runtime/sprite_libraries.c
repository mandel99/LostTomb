#include "sprite_libraries.h"
#include <string.h>
void restore_sprite_libraries_initialize(RestoreSpriteLibraries *self)
{
    static const uint32_t limits[30]={0x27,0xfd,0x10e,0x1f2,0x234,0x235,0x236,0x237,0x238,0x239,
        0x240,0x241,0x27a,0x282,0x293,0x2c7,0x2f0,0x2fc,0x2fd,0x2fe,0x2ff,0x300,0x301,0x312,
        0x317,0x32c,0x33c,0x353,0x370,0x393};
    /* Fresh-object helper for isolated reader fixtures. Game startup uses
       53DA50/53DA90, whose native contract preserves untouched arrays. */
    memset(self,0,sizeof(*self));
    memcpy(self->tag_limits,limits,sizeof(limits));self->next_tag=self->next_image=1;
}
