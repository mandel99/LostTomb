#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define actor_group (*(int32_t *)0x00bcd714u)
#define group_children (*(int32_t *)0x009784dcu)
#define building_x (*(int16_t *)0x007999deu)
#define building_y (*(int16_t *)0x007999e0u)
#define actor_active (*(uint8_t *)0x00bcd606u)
#define actor_kind (*(uint8_t *)0x00bcd602u)
#define actor_state (*(uint8_t *)0x00bcd630u)
#define actor_saved_state (*(uint8_t *)0x00bcd608u)
#define actor_marker (*(uint8_t *)0x00bcd631u)
#define actor_next_x (*(int16_t *)0x00bcd610u)
#define actor_next_y (*(int16_t *)0x00bcd612u)
#define actor_x (*(int16_t *)0x00bcd60cu)
#define actor_y (*(int16_t *)0x00bcd60eu)
#define actor_tile (*(uint32_t *)0x00bcd618u)
#define actor_pixel_x (*(int16_t *)0x00bcd642u)
#define actor_pixel_y (*(int16_t *)0x00bcd644u)
#define placement_x (*(int32_t *)0x00d3b4e4u)
#define placement_y (*(int32_t *)0x00d3b4e0u)
#define city_origin (*(uint32_t *)0x00bca2d8u)

extern void FUN_00480180(int32_t x, int32_t y, int32_t radius);
extern int32_t FUN_00492360(int32_t actor);
extern int32_t FUN_0049b3c0(int32_t actor);
extern int32_t FUN_004a34b0(int32_t actor);
extern int32_t FUN_004a3540(int32_t actor);
extern void FUN_004b52f0(int32_t actor);

void FUN_004a38a0(int32_t parent, int32_t building)
{
    int32_t group = (&actor_group)[(uint32_t)parent * 97u];
    int32_t remaining = FUN_004a34b0(parent);
    if (remaining > 0) {
        int32_t *list = &group_children + group * 11;
        do {
            int32_t child = *list;
            *list = -1;
            uint32_t offset = (uint32_t)child * 388u;
            if ((&actor_active)[offset] == 1 && (&actor_state)[offset] == 6) {
                FUN_00480180((&building_x)[(uint32_t)building * 132u],
                    (&building_y)[(uint32_t)building * 132u], 2);
                int32_t x = placement_x, y = placement_y;
                uint32_t word = (uint32_t)child * 194u;
                (&actor_next_x)[word] = (int16_t)x;
                (&actor_x)[word] = (int16_t)x;
                (&actor_next_y)[word] = (int16_t)y;
                (&actor_y)[word] = (int16_t)y;
                (&actor_tile)[(uint32_t)child * 97u] = city_origin + (uint32_t)y * 228u + (uint32_t)x;
                (&actor_pixel_x)[word] = (int16_t)((&actor_x)[word] * 15);
                (&actor_pixel_y)[word] = (int16_t)((&actor_y)[word] * 15);
                (&actor_state)[offset] = (&actor_saved_state)[offset];
                (&actor_marker)[offset] = 15;
                FUN_004b52f0(child);
                for (int32_t follower = 1; follower < 2000; ++follower) {
                    int32_t followed;
                    switch ((&actor_kind)[(uint32_t)follower * 388u]) {
                    case 0x15: followed = FUN_00492360(follower); break;
                    case 0x42: followed = FUN_0049b3c0(follower); break;
                    case 0x56:
                    case 0x60: followed = FUN_004a3540(follower); break;
                    default: continue;
                    }
                    if (followed == child) {
                        uint32_t fw = (uint32_t)follower * 194u;
                        int16_t child_x = (&actor_x)[word], child_y = (&actor_y)[word];
                        (&actor_next_x)[fw] = child_x;
                        (&actor_x)[fw] = child_x;
                        (&actor_next_y)[fw] = child_y;
                        (&actor_y)[fw] = child_y;
                        (&actor_tile)[(uint32_t)follower * 97u] = city_origin + (uint32_t)(int32_t)child_x
                            + (uint32_t)(int32_t)child_y * 228u;
                        (&actor_pixel_x)[fw] = (int16_t)((&actor_x)[fw] * 15);
                        (&actor_pixel_y)[fw] = (int16_t)((&actor_y)[fw] * 15);
                        /* Native retains ESI=child here, intentionally marking child again. */
                        (&actor_marker)[offset] = 15;
                        FUN_004b52f0(follower);
                    }
                }
            }
            ++list;
        } while (--remaining != 0);
    }
}
