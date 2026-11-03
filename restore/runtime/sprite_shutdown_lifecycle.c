#include "sprite_shutdown.h"
/* Original unnamed normal-path library constructor,005404e0. */
RestoreSpriteLibrary *FUN_005404e0(RestoreSpriteLibrary *self)
{
 self->pixels=NULL;self->name[0]=0;self->pixel_bytes=0;self->image_count=0;self->tag_count=0;return self;
}
/* Original registered global cleanup thunk,0053da80. */
void FUN_0053da80(void){FUN_0053db00(RESTORE_SPRITE_LIBRARIES);}

/* Last game CRT-table entry576078. Registration preserves the host status. */
int FUN_0053da50(void){(void)FUN_0053da60();return FUN_0053da70();}

void restore_sprite_libraries_destroy_records(RestoreSpriteLibraries *self)
{
    unsigned int count=30;
    while(count){--count;FUN_00540500(&self->libraries[count]);}
}
void restore_sprite_libraries_destroy_frame(uint32_t frame)
{
    /* Both56B770 and56B7A0 capture the manager once from the parent frame.
       These callbacks destroy records only; they do not reset manager state.
       Reconstructing callback bodies does not implement exception dispatch. */
    uint32_t manager=*(volatile uint32_t *)(uintptr_t)(frame-16u);
    restore_sprite_libraries_destroy_records((RestoreSpriteLibraries *)(uintptr_t)manager);
}
