#include "terrain_redraw.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define H(a) (*(uint16_t *)(uintptr_t)(a))
void FUN_0047c140(int32_t building,int32_t x,int32_t y,int32_t width,int32_t height,
                  uint32_t image,uint32_t flags)
{
    int32_t row,column,tile;
    uint32_t frame = 0;
    if (x < 0 || y < 0 ||
        (int32_t)((uint32_t)x + (uint32_t)width - 1u) >= I(0xbca2d0) ||
        (int32_t)((uint32_t)y + (uint32_t)height - 1u) >= I(0xbca2d4)) {
        U(0xbc9734) = 1;
        return;
    }
    tile = (int32_t)(U(0xbca2d8) + (uint32_t)y * 228u + (uint32_t)x);
    U(0xbc9768) = (uint32_t)tile;
    for (row = 0; row < height; ++row)
        for (column = 0; column < width; ++column) {
            uint32_t i = (uint32_t)tile + U(0x5cc8c0u + row * 48u + column * 8u);
            B(0xb285ccu + i) &= 0xe0;
            B(0xb67d1cu + i) |= 0x40;
            U(0xaf598cu + i * 4u) = (U(0xaf598cu + i * 4u) & 0x3052e80u) | flags;
            H(0xb4e6fcu + i * 2u) = (uint16_t)building;
            U(0xb7482cu + i * 4u) = image + frame++;
        }
}
