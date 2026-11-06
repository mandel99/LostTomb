#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define actor_building (*(int16_t *)0x00bcd656u)
#define actor_target_x (*(int16_t *)0x00bcd61cu)
#define actor_target_y (*(int16_t *)0x00bcd61eu)
#define actor_pixel_x (*(int16_t *)0x00bcd642u)
#define actor_pixel_y (*(int16_t *)0x00bcd644u)
#define actor_wait (*(uint16_t *)0x00bcd63cu)
#define building_kind (*(int16_t *)0x007999e8u)
#define building_stage (*(uint8_t *)0x00799a2du)
#define building_sibling (*(int16_t *)0x007999eau)
#define building_owner (*(int16_t *)0x00799a3cu)
#define tile_building (*(int16_t *)0x00b4e6fcu)
#define city_origin (*(uint32_t *)0x00bca2d8u)

extern void FUN_004b4220(int32_t actor, int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y, int32_t mode);
extern uint8_t FUN_004f3370(int32_t building, int32_t *x, int32_t *y, int32_t mode);

/* Native callers TEST AL. Higher EAX bits are not a logical API result. */
uint8_t FUN_004f4320(int32_t actor)
{
    uint32_t word = (uint32_t)actor * 194u;
    int32_t building = (&actor_building)[word];
    if ((&building_kind)[(uint32_t)building * 132u] != 0xb7 ||
        (&building_stage)[(uint32_t)building * 264u] < 2) return 0;
    int32_t target_x, target_y;
    if (FUN_004f3370(building - (&building_sibling)[(uint32_t)building * 132u],
            &target_x, &target_y, 1) == 0) return 0;
    (&actor_target_x)[word] = (int16_t)target_x;
    (&actor_target_y)[word] = (int16_t)target_y;
    FUN_004b4220(actor, (&actor_pixel_x)[word], (&actor_pixel_y)[word],
        (&actor_target_x)[word] * 15, (int16_t)target_y * 15, 0);
    (&actor_wait)[word] = 0;
    uint32_t tile = city_origin + (uint32_t)target_y * 228u + (uint32_t)target_x;
    int16_t destination_building = (&tile_building)[tile];
    (&actor_building)[word] = destination_building;
    (&building_owner)[(int32_t)destination_building * 132] = (int16_t)actor;
    return 1;
}
