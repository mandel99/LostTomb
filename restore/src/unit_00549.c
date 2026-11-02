#include "world_render.h"
/* Native thiscall method, RET4. The group table's image is the second dword
   of each8-byte entry; native code does not add range validation. */
uint32_t restore_world_image_base(RestoreSpriteLibraries *self,int32_t group)
{
    uint32_t address=(uint32_t)(uintptr_t)self+(uint32_t)group*8u+4u;
    return *(volatile uint32_t *)(uintptr_t)address;
}
uint32_t FUN_004381a0(int32_t group)
{
    return restore_world_image_base(RESTORE_SPRITE_LIBRARIES,group);
}
