#ifndef PHARAOH_RUNTIME_RANDOM_GENERATOR_H
#define PHARAOH_RUNTIME_RANDOM_GENERATOR_H

#include <stdint.h>

enum { PHARAOH_RANDOM_POOL_LENGTH = 100 };

typedef struct PharaohRandomGeneratorValues {
    uint32_t random_2_15_bit;
    uint32_t random_2_7_bit;
    uint32_t random_2_3_bit;
    uint32_t random_1_15_bit;
    uint32_t random_1_7_bit;
    uint32_t random_1_3_bit;
    uint32_t seed_3;
    uint32_t seed_2;
    uint32_t seed_1;
} PharaohRandomGeneratorValues;

typedef struct PharaohRandomGeneratorContext {
    PharaohRandomGeneratorValues *values;
    uint32_t *pool;
    int32_t *pool_index;
} PharaohRandomGeneratorContext;

/* One generator advance is inlined in native 0x004D7090. */
void pharaoh_random_generator_advance(
    const PharaohRandomGeneratorContext *context);
/* Native entry 0x004CDD60. */
void pharaoh_random_generator_fill_pool(
    const PharaohRandomGeneratorContext *context);

#endif
