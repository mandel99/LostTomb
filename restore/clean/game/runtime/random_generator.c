#include "pharaoh/runtime/random_generator.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohRandomGeneratorValues) == 0x24 &&
    offsetof(PharaohRandomGeneratorValues, random_1_7_bit) == 0x10 &&
    offsetof(PharaohRandomGeneratorValues, seed_2) == 0x1c &&
    offsetof(PharaohRandomGeneratorValues, seed_1) == 0x20,
    "Native random generator value layout");

enum {
    RANDOM_FEEDBACK_SHIFT = 4,
    RANDOM_FEEDBACK_TOP_BIT = 0x40000000,
    RANDOM_SHIFTS_PER_ADVANCE = 31,
    RANDOM_3_BIT_MASK = 7,
    RANDOM_7_BIT_MASK = 0x7f,
    RANDOM_15_BIT_MASK = 0x7fff
};

void pharaoh_random_generator_advance(
    const PharaohRandomGeneratorContext *context)
{
    PharaohRandomGeneratorValues *const values = context->values;
    context->pool[*context->pool_index] = values->random_1_7_bit;
    ++*context->pool_index;
    if (*context->pool_index >= PHARAOH_RANDOM_POOL_LENGTH) {
        *context->pool_index = 0;
    }

    for (int32_t shift = 0; shift < RANDOM_SHIFTS_PER_ADVANCE; ++shift) {
        const uint32_t feedback_1 =
            ((values->seed_1 >> RANDOM_FEEDBACK_SHIFT) ^ values->seed_1) & 1;
        const uint32_t feedback_2 =
            ((values->seed_2 >> RANDOM_FEEDBACK_SHIFT) ^ values->seed_2) & 1;
        values->seed_1 >>= 1;
        values->seed_2 >>= 1;
        if (feedback_1 != 0) {
            values->seed_1 |= RANDOM_FEEDBACK_TOP_BIT;
        }
        if (feedback_2 != 0) {
            values->seed_2 |= RANDOM_FEEDBACK_TOP_BIT;
        }
    }

    values->random_1_15_bit = values->seed_1 & RANDOM_15_BIT_MASK;
    values->random_1_7_bit = values->seed_1 & RANDOM_7_BIT_MASK;
    values->random_1_3_bit = values->seed_1 & RANDOM_3_BIT_MASK;
    values->random_2_15_bit = values->seed_2 & RANDOM_15_BIT_MASK;
    values->random_2_7_bit = values->seed_2 & RANDOM_7_BIT_MASK;
    values->random_2_3_bit = values->seed_2 & RANDOM_3_BIT_MASK;
}

void pharaoh_random_generator_fill_pool(
    const PharaohRandomGeneratorContext *context)
{
    *context->pool_index = 0;
    for (int32_t item = 0; item < PHARAOH_RANDOM_POOL_LENGTH; ++item) {
        pharaoh_random_generator_advance(context);
    }
}

