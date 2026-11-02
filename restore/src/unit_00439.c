#include "terrain_diamond.h"
void FUN_0042b440(const void *pixels,int clip)
{
    TERRAIN_TRACE(0x42b440,pixels,clip);
    if(*(uint16_t *)(uintptr_t)0x70e120)FUN_0042bcf0(pixels,clip);
    else restore_terrain_diamond(pixels,clip,2,0);
}
