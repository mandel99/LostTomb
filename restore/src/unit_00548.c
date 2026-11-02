#include "world_render.h"
/* Native thiscall method, RET4. Signed comparison and address arithmetic
   preserve the original index0 substitution and 32-bit wrapping. */
int32_t restore_world_image_record(RestoreSpriteLibraries *self,int32_t image)
{
    uint32_t index=(uint32_t)image;
    if(image>=(int32_t)*(volatile uint32_t *)((uint8_t *)self+0x30f5cc))index=0;
    return (int32_t)((uint32_t)(uintptr_t)self+0x180e48u+index*64u);
}
int32_t FUN_00438180(int32_t image)
{
    return restore_world_image_record(RESTORE_SPRITE_LIBRARIES,image);
}
