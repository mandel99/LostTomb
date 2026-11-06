#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define city_height (*(int32_t *)0x00bca2d4u)
#define map_turn (*(uint32_t *)0x00bc96c8u)
#define row_tile (*(int32_t *)0x00bc9804u)
#define row_left (*(int32_t *)0x00bc9f24u)
#define row_right (*(int32_t *)0x00bc9b94u)
#define tile_flags (*(uint32_t *)0x00af598cu)
#define tile_direction (*(uint8_t *)0x00adc36cu)

void FUN_0047fb70(void)
{
    int32_t height = city_height;
    uint32_t turn = map_turn;
    for (int32_t row = 0; row < height; ++row) {
        if ((&row_left)[row] < (&row_right)[row]) {
            int32_t tile = (&row_tile)[row];
            int32_t remaining = (&row_right)[row] - (&row_left)[row];
            do {
                uint8_t direction = (&tile_direction)[tile];
                if (((&tile_flags)[tile] & 4u) != 0 && direction != 0) {
                    switch (direction) {
                    case 1: direction = turn == 1 ? 6 : 4; break;
                    case 2: direction = 5; break;
                    case 3: direction = turn == 1 ? 4 : 6; break;
                    case 4: direction = turn == 1 ? 1 : 3; break;
                    case 5: direction = 2; break;
                    case 6: direction = turn == 1 ? 3 : 1; break;
                    default: goto next_tile;
                    }
                    (&tile_direction)[tile] = direction;
                }
next_tile:
                ++tile;
            } while (--remaining != 0);
        }
    }
}
