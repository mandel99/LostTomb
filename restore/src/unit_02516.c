#include "sprite_shutdown.h"
uint32_t FUN_0053db60(RestoreSpriteLibraries *self)
{
 static const uint32_t limits[30]={0x27,0xfd,0x10e,0x1f2,0x234,0x235,0x236,0x237,0x238,0x239,0x240,0x241,0x27a,0x282,0x293,0x2c7,0x2f0,0x2fc,0x2fd,0x2fe,0x2ff,0x300,0x301,0x312,0x317,0x32c,0x33c,0x353,0x370,0x393};
 unsigned i;FUN_0053f790((int *)(void *)&self->alternate,0);FUN_0053f790((int *)(void *)&self->scratch,0);
 for(i=0;i<30;++i){RestoreSpriteLibrary *library=&self->libraries[i];if(library->pixels){restore_sprite_shutdown_free(library->pixels);library->pixels=NULL;}
  library->name[0]=0;library->pixel_bytes=0;library->image_count=0;library->tag_count=0;self->tag_limits[i]=limits[i];
 }
 self->loaded=0;self->next_tag=1;self->next_image=1;self->pixel_bytes=0;return 1;
}
