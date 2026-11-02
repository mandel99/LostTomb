#include "terrain_redraw.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
int32_t FUN_0047c880(int32_t x,int32_t y,int32_t width,int32_t height,
                    int32_t mask,int32_t required_image,int32_t allow_floodplain)
{
    int32_t row,column;uint32_t seen_flood = 0, seen_dry = 0;
    if (x < 3 || y < 3 ||
        (int32_t)((uint32_t)x + (uint32_t)width - 1u) >= (int32_t)(U(0xbca2d0)-3u) ||
        (int32_t)((uint32_t)y + (uint32_t)height - 1u) >= (int32_t)(U(0xbca2d4)-3u)) return 0;
    U(0xbc9768) = U(0xbca2d8) + (uint32_t)y * 228u + (uint32_t)x;
    for (row = 0; row < height; ++row)
        for (column = 0; column < width; ++column) {
            int32_t tile = (int32_t)(U(0xbc9768) + U(0x5cc8c0u + row * 48u + column * 8u));
            uint32_t flags,object;
            if ((uint8_t)FUN_0046f9a0(tile % 228,tile / 228,3)) return 0;
            flags = U(0xaf598cu + (uint32_t)tile * 4u) & ((uint32_t)mask | 0x10010000u);
            object = U(0xd05720u + (uint32_t)tile * 4u);
            if (object && !B(object + 0x18u)) return 0;
            if (flags & 0x10000u) {if (seen_dry) return 0;seen_flood = 1;}
            else {if (seen_flood) return 0;seen_dry = 1;}
            flags &= (uint8_t)allow_floodplain ? 0xfefed77fu : 0xfeffd77fu;
            if (flags || H(0xb350dcu + (uint32_t)tile * 2u)) return 0;
            if (required_image && U(0xb7482cu + (uint32_t)tile * 4u) != (uint32_t)required_image - 1u) return 0;
        }
    return 1;
}
