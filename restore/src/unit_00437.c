#include "terrain_diamond.h"
void FUN_0042a160(const void *pixels,int clip)
{
    TERRAIN_TRACE(0x42a160,pixels,clip);
    if(*(uint16_t *)(uintptr_t)0x70e120)FUN_0042aa00(pixels,clip);
    else restore_terrain_diamond(pixels,clip,1,0);
}
