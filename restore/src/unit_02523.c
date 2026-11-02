#include "message_panel.h"
uint8_t restore_message_sprite_dimensions(RestoreSpriteLibraries *self,int32_t image,int32_t *width,int32_t *height)
{
    int32_t index=image;const uint8_t *record;
    *height=0;*width=0;
    if((int32_t)self->next_image<=image)index=0;
    record=(const uint8_t *)(uintptr_t)((uint32_t)(uintptr_t)self+0x180e48u+(uint32_t)index*64u);
    if(image>0&&image<25500&&record) {
        *width=*(const int16_t *)(record+20);*height=*(const int16_t *)(record+22);return 1;
    }
    return 0;
}
uint8_t FUN_0053e530(int32_t image,int32_t *width,int32_t *height)
{return restore_message_sprite_dimensions(RESTORE_SPRITE_LIBRARIES,image,width,height);}
