#include "terrain_redraw.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
void FUN_0047db00(uint32_t mask)
{
    static const int32_t offsets[8] = {-228,-227,1,229,228,227,-1,-229};
    uint32_t i,tile = U(0xbc9768);
    for (i = 0; i < 8; ++i)
        U(0xbc97c0u + i * 4u) = !!(U(0xaf598cu + (tile + (uint32_t)offsets[i]) * 4u) & mask);
}
