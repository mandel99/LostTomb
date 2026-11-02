#include "city_prepare.h"
#include "terrain_redraw.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) ((int32_t)U(a))
uint32_t FUN_0046f9a0(int32_t x,int32_t y,int32_t margin)
{
    uint32_t eax = FUN_0046f930(x,y), value, bound, condition;
    if (!(uint8_t)eax) return eax;
    if (x <= I(0xbca2f8)) {
        if (y <= I(0xbca2fc)) {
            eax = U(0xbca2e8) + U(0xbca2fc);
            value = (uint32_t)x - (uint32_t)margin + (uint32_t)y;
        } else {
            eax = U(0xbca2f8) - U(0xbca2f4);
            value = (uint32_t)x - (uint32_t)y - (uint32_t)margin;
        }
        condition = (int32_t)value < (int32_t)eax;
    } else {
        eax = U(0xbca2fc);
        if (y <= I(0xbca2fc)) {
            bound = U(0xbca2f0) - eax;
            value = (uint32_t)x - (uint32_t)y + (uint32_t)margin;
        } else {
            eax += U(0xbca2f0) + 1u;
            bound = eax;
            value = (uint32_t)x + (uint32_t)y + (uint32_t)margin;
        }
        condition = (int32_t)value > (int32_t)bound;
    }
    return (eax & 0xffffff00u) | condition;
}
