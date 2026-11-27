#ifndef PHARAOH_POPULATION_DISPLACED_H
#define PHARAOH_POPULATION_DISPLACED_H

#include "pharaoh/population/age_counts.h"
#include "pharaoh/walker/create.h"
#include <stddef.h>

/* Actual shared city bank EA1870, not a detached population snapshot. */
typedef struct PharaohDisplacedPopulationCity {
    uint8_t reserved_0000[0x24];
    int32_t population;
    uint8_t reserved_0028[0x25c4-0x28];
    int16_t people_at_age[100];
    uint8_t reserved_268c[0x26f8-0x268c];
    int32_t displaced_population;
    uint8_t reserved_26fc[0x469c-0x26fc];
    int32_t highest_population;
    uint8_t reserved_46a0[0x49d8-0x46a0];
} PharaohDisplacedPopulationCity;

_Static_assert(sizeof(PharaohDisplacedPopulationCity)==0x49d8 &&
    offsetof(PharaohDisplacedPopulationCity,population)==0x24 &&
    offsetof(PharaohDisplacedPopulationCity,people_at_age)==0x25c4 &&
    offsetof(PharaohDisplacedPopulationCity,displaced_population)==0x26f8 &&
    offsetof(PharaohDisplacedPopulationCity,highest_population)==0x469c,
    "native population city fields");

typedef struct PharaohDisplacedPopulationContext {
    PharaohWalkerCreateContext *walker_create;
    PharaohDisplacedPopulationCity *cities;
    const uint32_t *random_pool;
    const int32_t *random_pool_index;
    const int32_t *map_origin;
} PharaohDisplacedPopulationContext;

/* Whole00459020. The actor's people field narrows to BYTE; the census uses
 * the original full people/player arguments. Native also writes actor0 when
 * allocation fails. All providers and buffers must belong to the live scene. */
void pharaoh_city_spawn_displaced_population(
    const PharaohDisplacedPopulationContext *,int32_t player,
    int32_t x,int32_t y,int32_t people);

#endif
