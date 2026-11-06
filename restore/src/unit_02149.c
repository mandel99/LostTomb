#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define actor_x (*(int16_t *)0x00bcd60cu)
#define actor_y (*(int16_t *)0x00bcd60eu)
#define actor_target_x (*(int16_t *)0x00bcd61cu)
#define actor_target_y (*(int16_t *)0x00bcd61eu)
#define actor_wait (*(int16_t *)0x00bcd62eu)
#define building_owner (*(int16_t *)0x00799a3cu)
#define tile_building (*(int16_t *)0x00b4e6fcu)
#define city_origin (*(uint32_t *)0x00bca2d8u)

extern int32_t FUN_004f3690(int32_t x, int32_t y);
extern int32_t FUN_004f4420(uint32_t tile);

uint8_t FUN_004f4440(int32_t actor)
{
    uint32_t word = (uint32_t)actor * 194u;
    uint32_t tile = city_origin + (uint32_t)(int32_t)(&actor_x)[word]
        + (uint32_t)(int32_t)(&actor_y)[word] * 228u;
    int16_t building = (&tile_building)[tile];
    if (building == 0) return 0;
    int32_t delay = FUN_004f4420(tile);
    (&actor_wait)[word] = (int16_t)((uint16_t)(&actor_wait)[word] + 1u);
    if (delay <= (&actor_wait)[word]) {
        (void)FUN_004f3690((&actor_target_x)[word], (&actor_target_y)[word]);
        (&building_owner)[(int32_t)building * 132] = 0;
        return 0;
    }
    return 1;
}
