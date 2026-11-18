#include "pharaoh/runtime/animation_tick_counters.h"

static void advance_counter(int32_t *counter, int32_t period)
{
    const uint32_t next_bits = (uint32_t)*counter + UINT32_C(1);
    *counter = (int32_t)next_bits;
    if ((int32_t)next_bits >= period) {
        *counter = 0;
    }
}

void pharaoh_animation_tick_counters_advance(
    PharaohAnimationTickCounters *counters)
{
    advance_counter(&counters->every_4_ticks, 4);
    advance_counter(&counters->every_8_ticks, 8);
    advance_counter(&counters->every_16_ticks, 16);
    advance_counter(&counters->every_24_ticks, 24);
    advance_counter(&counters->every_32_ticks, 32);
    advance_counter(&counters->every_64_ticks, 64);
    advance_counter(&counters->every_128_ticks, 128);
    advance_counter(&counters->every_256_ticks, 256);
    advance_counter(&counters->every_384_ticks, 384);
    advance_counter(&counters->every_768_ticks, 768);
}
