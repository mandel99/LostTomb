#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define sprite_base (*(uint32_t *)0x00f565f4u)
#define city_height (*(int32_t *)0x00bca2d4u)
#define row_tile (*(int32_t *)0x00bc9804u)
#define row_left (*(int32_t *)0x00bc9f24u)
#define row_right (*(int32_t *)0x00bc9b94u)
#define tile_sprite (*(uint32_t *)0x00b7482cu)
#define tile_flags (*(uint32_t *)0x00af598cu)
#define tile_terrain (*(uint8_t *)0x00b285ccu)
#define tile_overlay (*(uint8_t *)0x00ae8e7cu)

void FUN_00476750(void)
{
    int32_t first_sprite = (int32_t)sprite_base;
    for (int32_t row = 0; row < city_height; ++row) {
        int32_t tile = (&row_tile)[row];
        int32_t left = (&row_left)[row], right = (&row_right)[row];
        if (left < right) {
            int32_t remaining = right - left;
            do {
                int32_t sprite = (int32_t)(&tile_sprite)[tile];
                if (first_sprite <= sprite && sprite <= (int32_t)((uint32_t)first_sprite + 7u)) {
                    (&tile_flags)[tile] |= 0x20u;
                    (&tile_terrain)[tile] &= 0x7fu;
                    (&tile_overlay)[tile] = 0;
                }
                ++tile;
            } while (--remaining != 0);
        }
    }
}
