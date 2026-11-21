#ifndef PHARAOH_CORE_CRT_RANDOM_H
#define PHARAOH_CORE_CRT_RANDOM_H

#include <stdint.h>

typedef struct PharaohCrtRandom {
    uint32_t seed;
} PharaohCrtRandom;

/* Complete C translations of Pharaoh.exe 0x0056233E and 0x00562348. */
void pharaoh_crt_random_seed(PharaohCrtRandom *random, uint32_t seed);
uint32_t pharaoh_crt_random_next(PharaohCrtRandom *random);

#endif
