#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define city_origin (*(uint32_t *)0x00bca2d8u)
#define current_actor (*(uint32_t *)0x00d056f8u)
#define actor_active (*(uint8_t *)0x00bcd606u)
#define actor_kind (*(uint8_t *)0x00bcd602u)
#define actor_state (*(uint8_t *)0x00bcd630u)
#define actor_owner (*(int32_t *)0x00bcd730u)
#define actor_reference (*(int16_t *)0x00bcd652u)
#define actor_x (*(int16_t *)0x00bcd60cu)
#define actor_y (*(int16_t *)0x00bcd60eu)
#define actor_distance (*(int16_t *)0x00bcd636u)

extern void FUN_004b3250(int32_t actor);
extern uint8_t FUN_004ea9a0(int32_t kind);
extern int32_t FUN_005190a0(uint32_t request, uint32_t target, const uint32_t *tiles, int32_t count);

int32_t FUN_004a2320(uint32_t request, uint32_t target, int32_t maximum_distance)
{
    /* Native ESP+0..239 is one 60-DWORD allocation: tiles[30], then IDs[30]. */
    uint32_t candidates[60];
    int32_t count = 0;
    for (int32_t actor = 1; actor < 2000; ++actor) {
        uint32_t offset = (uint32_t)actor * 388u;
        if ((&actor_active)[offset] == 1 && FUN_004ea9a0((&actor_kind)[offset]) != 0) {
            uint8_t state = (&actor_state)[offset];
            int32_t owner = (&actor_owner)[(uint32_t)actor * 97u];
            if (state != 4 && state != 12 &&
                (owner == 0 || owner == (&actor_reference)[current_actor * 194u])) {
                candidates[count] = city_origin + (uint32_t)(int32_t)(&actor_x)[(uint32_t)actor * 194u]
                    + (uint32_t)(int32_t)(&actor_y)[(uint32_t)actor * 194u] * 228u;
                candidates[count + 30] = (uint32_t)actor;
                if (++count == 30) break;
            }
        }
    }
    if (count != 0) {
        int32_t selected = FUN_005190a0(request, target, candidates, count);
        if (selected != 0 && (&actor_distance)[current_actor * 194u] <= maximum_distance) {
            FUN_004b3250((int32_t)current_actor);
            return (int32_t)candidates[selected + 29];
        }
    }
    return 0;
}
