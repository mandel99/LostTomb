#ifndef PHARAOH_RUNTIME_ANIMATION_TICK_COUNTERS_H
#define PHARAOH_RUNTIME_ANIMATION_TICK_COUNTERS_H

#include <stdint.h>

typedef struct PharaohAnimationTickCounters {
    int32_t every_4_ticks;
    int32_t every_8_ticks;
    int32_t every_16_ticks;
    int32_t every_24_ticks;
    int32_t every_32_ticks;
    int32_t every_64_ticks;
    int32_t every_128_ticks;
    int32_t every_256_ticks;
    int32_t every_384_ticks;
    int32_t every_768_ticks;
} PharaohAnimationTickCounters;

/* Native entry 0x004CDC70. */
void pharaoh_animation_tick_counters_advance(
    PharaohAnimationTickCounters *counters);

#endif
