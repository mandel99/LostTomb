#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define city_width (*(int32_t *)0x00bca2d0u)
#define city_height (*(int32_t *)0x00bca2d4u)
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define row_skip (*(int32_t *)0x00bca2dcu)
#define mask_buffer (*(uint32_t *)0x00e94dd4u)
#define mask_buffer_offset (*(uint32_t *)0x00e94dd8u)
#define mask_x (*(int32_t *)0x005e2950u)
#define mask_y (*(int32_t *)0x005e2954u)
#define tile_flags (*(uint32_t *)0x00af598cu)
#define tile_terrain (*(uint8_t *)0x00b285ccu)

void FUN_004de200(int32_t mask_index)
{
    int32_t width = city_width;
    if (city_height > 0) {
        uint32_t source = mask_buffer + (uint32_t)(&mask_y)[(uint32_t)mask_index * 2u] * 1000u
            + mask_buffer_offset + (uint32_t)(&mask_x)[(uint32_t)mask_index * 2u];
        int32_t rows = city_height;
        uint32_t tile = city_origin;
        do {
            for (int32_t x = 0; x < width; ++x) {
                uint8_t value = *(uint8_t *)(uintptr_t)(source + (uint32_t)x);
                if (value == 16) (&tile_flags)[tile] = 2;
                else if (value == 11) (&tile_flags)[tile] = 1;
                else if (value == 4) (&tile_flags)[tile] = 4;
                else if (value == 8) (&tile_flags)[tile] = 0x40;
                else if (value == 3) (&tile_terrain)[tile] |= 0x20u;
                ++tile;
            }
            source += 1000u;
            tile += (uint32_t)row_skip;
        } while (--rows != 0);
    }
}
