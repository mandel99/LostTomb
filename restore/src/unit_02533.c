#include "sprite_shutdown.h"
void FUN_00540500(RestoreSpriteLibrary *self)
{
 if(self->pixels){restore_sprite_shutdown_free(self->pixels);self->pixels=NULL;}self->name[0]=0;self->pixel_bytes=0;self->image_count=0;self->tag_count=0;
}
