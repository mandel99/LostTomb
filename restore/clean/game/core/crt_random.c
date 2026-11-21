#include "pharaoh/core/crt_random.h"

void pharaoh_crt_random_seed(PharaohCrtRandom *random, uint32_t seed)
{
    random->seed = seed;
}

uint32_t pharaoh_crt_random_next(PharaohCrtRandom *random)
{
    random->seed = random->seed * UINT32_C(214013) + UINT32_C(2531011);
    return (random->seed >> 16) & UINT32_C(0x7fff);
}
