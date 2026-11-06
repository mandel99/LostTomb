#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define selection_mode (*(uint8_t *)0x00e38ddau)
#define selected_formation (*(uint32_t *)0x00578c6cu)
#define formation_active (*(uint8_t *)0x00bcb9c8u)
#define formation_busy (*(uint8_t *)0x00bcba31u)
#define formation_count (*(uint8_t *)0x00bcba0au)
#define formation_x (*(int16_t *)0x00bcb9f6u)
#define formation_y (*(int16_t *)0x00bcb9f8u)
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define selection_dirty (*(uint8_t *)0x00e38dcau)

extern void FUN_00430cd0(uint32_t tile);

void FUN_00416a20(void)
{
    if (selection_mode == 1) {
        uint32_t chosen = 0;
        for (int remaining = 6; remaining != 0; --remaining) {
            ++selected_formation;
            if ((int32_t)selected_formation > 6) selected_formation = 1;
            uint32_t offset = selected_formation * 144u;
            if ((&formation_active)[offset] == 1 &&
                (&formation_busy)[offset] == 0 &&
                (&formation_count)[offset] != 0 && chosen == 0)
                chosen = selected_formation;
        }
        selected_formation = chosen;
        if ((int32_t)chosen > 0) {
            uint32_t word_offset = chosen * 72u;
            uint32_t tile = city_origin + (uint32_t)(int32_t)(&formation_x)[word_offset]
                + (uint32_t)(int32_t)(&formation_y)[word_offset] * 228u;
            FUN_00430cd0(tile);
            selection_dirty = 1;
        }
    }
}
