#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define building_x (*(int16_t *)0x007999deu)
#define building_y (*(int16_t *)0x007999e0u)
#define building_dirty (*(uint8_t *)0x00799a17u)
#define tile_building (*(int16_t *)0x00b4e6fcu)
#define city_width (*(int32_t *)0x00bca2d0u)
#define city_height (*(int32_t *)0x00bca2d4u)
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define scan_y (*(int32_t *)0x00d3b4e0u)
#define scan_x (*(int32_t *)0x00d3b4e4u)

void FUN_0047fc40(int32_t building, int32_t radius)
{
    int32_t x = (&building_x)[(uint32_t)building * 132u];
    int32_t y = (&building_y)[(uint32_t)building * 132u];
    int32_t left = x - radius, right = x + radius;
    int32_t bottom = y + radius;
    scan_y = y - radius;
    if (left < 0) left = 0;
    if (scan_y < 0) scan_y = 0;
    if (city_width <= right) right = city_width - 1;
    if (city_height <= bottom) bottom = city_height - 1;
    uint32_t tile = city_origin + (uint32_t)scan_y * 228u + (uint32_t)left;
    if (scan_y <= bottom) {
        int32_t rows = bottom - scan_y + 1;
        scan_y += rows;
        do {
            scan_x = left;
            if (left <= right) {
                int16_t *building_at = &tile_building + tile;
                int32_t columns = right - left + 1;
                tile += (uint32_t)columns;
                scan_x = columns + left;
                do {
                    int16_t occupant = *building_at++;
                    if (occupant != 0) (&building_dirty)[(int32_t)occupant * 264] = 1;
                } while (--columns != 0);
            }
            tile += (uint32_t)(left - right) + 227u;
        } while (--rows != 0);
    }
}
