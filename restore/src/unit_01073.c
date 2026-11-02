#include "terrain_redraw.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole478420: both dimensions are DWORDs, with native subtract wrapping. */
void FUN_00478420(void)
{
    uint32_t bottom=U(0xbca2d4u)-1u,right=U(0xbca2d0u)-1u;
    FUN_00478330(0,0,(int32_t)right,(int32_t)bottom);
}
