#include "terrain_diamond.h"
void FUN_00427a20(const void *pixels,int clip)
{
    TERRAIN_TRACE(0x427a20,pixels,clip);
    if(*(uint16_t *)(uintptr_t)0x70e120)FUN_00428c00(pixels,clip);
    else restore_terrain_diamond(pixels,clip,0,0);
}
