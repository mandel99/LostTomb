#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define tile_building (*(int16_t *)0x00b4e6fcu)
#define building_tile (*(uint32_t *)0x007999e4u)
#define building_sibling (*(int16_t *)0x007999eau)
#define building_stage (*(uint8_t *)0x00799a2du)
#define building_mask (*(uint8_t *)0x00799a33u)
#define corner_offsets (*(int32_t *)0x005f6bdcu)
#define sprite_base (*(uint32_t *)0x00f56b54u)
#define tile_flags (*(uint32_t *)0x00af598cu)
#define tile_sprite (*(uint32_t *)0x00b7482cu)
#define tile_noise (*(uint8_t *)0x00acf85cu)

extern void FUN_004ee4a0(int32_t building);
extern void FUN_004ee630(int32_t building);
extern uint8_t FUN_004f3360(uint32_t stage);

int32_t FUN_004f3690(int32_t x, int32_t y)
{
    uint32_t tile = city_origin + (uint32_t)y * 228u + (uint32_t)x;
    int32_t building = (&tile_building)[tile];
    uint32_t offset = (uint32_t)building * 264u;
    uint8_t bit = 0;
    for (uint32_t corner = 0; corner < 4; ++corner) {
        if ((&building_tile)[(uint32_t)building * 66u] + (uint32_t)(&corner_offsets)[corner] == tile) {
            bit = (uint8_t)(1u << corner);
            break;
        }
    }
    uint8_t mask = (&building_mask)[offset];
    if (mask == 15) {
        uint8_t stage = (&building_stage)[offset];
        if (stage < 10) {
            uint8_t advanced = FUN_004f3360(stage);
            (&building_stage)[offset] = advanced;
            if (advanced == 5) bit = 15;
            (&building_mask)[offset] = bit;
        } else {
            if ((&building_sibling)[(uint32_t)building * 132u] != 0) return 0;
            if ((uint32_t)stage + 1u < 12u) {
                for (int32_t part = 0; part < 4; ++part) {
                    uint32_t part_offset = (uint32_t)(building + part) * 264u;
                    (&building_stage)[part_offset] = (uint8_t)(stage + 1u);
                    (&building_mask)[part_offset] = 15;
                }
                FUN_004ee4a0(building);
                FUN_004ee630(building);
                return building;
            }
        }
    } else {
        if ((&building_stage)[offset] == 5) bit = 15;
        (&building_mask)[offset] = (uint8_t)(mask | bit);
        uint32_t base_sprite = sprite_base;
        if ((&building_stage)[offset] == 3) {
            (&building_stage)[offset] = 4;
            uint32_t origin = (&building_tile)[(uint32_t)building * 66u];
            for (uint32_t corner = 0; corner < 4; ++corner) {
                uint32_t corner_tile = origin + (uint32_t)(&corner_offsets)[corner];
                uint8_t noise = (&tile_noise)[corner_tile];
                (&tile_flags)[corner_tile] = 0x10000008u;
                (&tile_sprite)[corner_tile] = base_sprite + (noise & 7u);
            }
        }
    }
    uint32_t base_sprite = sprite_base;
    if ((&building_stage)[offset] < 5) {
        uint8_t noise = (&tile_noise)[tile];
        (&tile_flags)[tile] = 0x10000008u;
        (&tile_sprite)[tile] = base_sprite + (noise & 7u);
    }
    FUN_004ee630(building - (&building_sibling)[(uint32_t)building * 132u]);
    FUN_004ee4a0(building - (&building_sibling)[(uint32_t)building * 132u]);
    return building;
}
