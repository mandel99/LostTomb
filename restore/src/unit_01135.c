#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define map_rotation (*(int32_t *)0x00e94e04u)
#define city_height (*(int32_t *)0x00bca2d4u)
#define row_tile (*(int32_t *)0x00bc9804u)
#define row_left (*(int32_t *)0x00bc9f24u)
#define row_right (*(int32_t *)0x00bc9b94u)
#define tile_terrain (*(uint8_t *)0x00b285ccu)
#define tile_marker (*(uint8_t *)0x00b67d1cu)

void FUN_0047d720(void)
{
    int32_t rotation = map_rotation;
    for (int32_t row = 0; row < city_height; ++row) {
        int32_t tile = (&row_tile)[row];
        if ((&row_left)[row] < (&row_right)[row]) {
            int32_t remaining = (&row_right)[row] - (&row_left)[row];
            do {
                uint8_t kind = (&tile_terrain)[tile] & 15u;
                uint8_t marker = (&tile_marker)[tile];
                uint8_t position = marker & 63u;
                if (kind == 0) (&tile_marker)[tile] = (uint8_t)(marker | 64u);
                else {
                    marker &= 0xbfu;
                    (&tile_marker)[tile] = marker;
                    if (kind <= 5) {
                        bool matches = (rotation == 0 && position == kind * 8u)
                            || (rotation == 6 && position == kind * 9u)
                            || (rotation == 4 && position == kind)
                            || (rotation == 2 && position == 0);
                        if (matches) (&tile_marker)[tile] = (uint8_t)(marker | 64u);
                    }
                }
                ++tile;
            } while (--remaining != 0);
        }
    }
}
