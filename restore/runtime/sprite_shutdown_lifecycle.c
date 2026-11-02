#include "sprite_shutdown.h"
/* Original unnamed normal-path library constructor,005404e0. */
RestoreSpriteLibrary *FUN_005404e0(RestoreSpriteLibrary *self)
{
 self->pixels=NULL;self->name[0]=0;self->pixel_bytes=0;self->image_count=0;self->tag_count=0;return self;
}
/* Original registered global cleanup thunk,0053da80. */
void FUN_0053da80(void){FUN_0053db00(RESTORE_SPRITE_LIBRARIES);}
