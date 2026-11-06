#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define building_bound (*(int32_t *)0x0078d790u)
#define building_timer (*(uint32_t *)0x00799bc8u)
#define actor_active (*(uint8_t *)0x00bcd606u)
#define actor_kind (*(uint8_t *)0x00bcd602u)
#define actor_state (*(uint8_t *)0x00bcd630u)
#define actor_wait (*(uint16_t *)0x00bcd62eu)
#define actor_target_x (*(uint16_t *)0x00bcd61cu)
#define actor_target_y (*(uint16_t *)0x00bcd61eu)
#define actor_saved_x (*(uint16_t *)0x00bcd624u)
#define actor_saved_y (*(uint16_t *)0x00bcd626u)
#define city_rate_a (*(uint32_t *)0x0078d7acu)
#define city_rate_b (*(uint32_t *)0x0078d7b0u)

extern void FUN_004b3250(int32_t actor);
extern void FUN_004b5420(int32_t actor);
extern void FUN_004e8e80(int32_t delta);

void FUN_004e7200(int32_t rate)
{
    if (rate < 3) rate = 3;
    else if (rate > 9) rate = 9;
    int32_t remaining = building_bound;
    if (remaining > 0) {
        uint32_t *timers = &building_timer;
        do {
            if (*(uint8_t *)((uint8_t *)timers - 0xe3) != 0) {
                int32_t denominator = (int32_t)(timers[3] + 1u);
                timers[0] += (uint32_t)(5000 / denominator);
                /* Native repeats IDIV after the first store. */
                denominator = (int32_t)(timers[3] + 1u);
                timers[5] += (uint32_t)(5000 / denominator);
                int32_t triple = (int32_t)(timers[3] * 3u);
                uint8_t *limit = (uint8_t *)timers - 0x71;
                if (triple < (int32_t)*limit) *limit = (uint8_t)triple;
            }
            timers += 66;
        } while (--remaining != 0);
    }
    for (int32_t actor = 1; actor < 2000; ++actor) {
        uint32_t offset = (uint32_t)actor * 388u;
        if ((&actor_active)[offset] != 0) {
            uint8_t kind = (&actor_kind)[offset];
            if (kind == 0x5f) FUN_004b5420(actor);
            else if (kind == 0x19 && ((&actor_state)[offset] == 10 || (&actor_state)[offset] == 9)) {
                (&actor_wait)[(uint32_t)actor * 194u] = 0;
                (&actor_state)[offset] = 13;
                (&actor_target_x)[(uint32_t)actor * 194u] = (&actor_saved_x)[(uint32_t)actor * 194u];
                (&actor_target_y)[(uint32_t)actor * 194u] = (&actor_saved_y)[(uint32_t)actor * 194u];
                FUN_004b3250(actor);
            }
        }
    }
    city_rate_b = (uint32_t)rate * 0x330u;
    city_rate_a = city_rate_b;
    FUN_004e8e80(-15);
}
