#ifndef PHARAOH_POPULATION_AGE_COUNTS_H
#define PHARAOH_POPULATION_AGE_COUNTS_H

#include <stdint.h>

enum { PHARAOH_CENSUS_ACADEMY_AGE_END = 21 };

enum { PHARAOH_CENSUS_AGE_COUNT = 100 };

typedef struct PharaohDisplacedPopulationState {
    int32_t *displaced_population;
    int32_t *population;
    int16_t *people_at_age;
    int32_t *highest_population;
} PharaohDisplacedPopulationState;

/* Original addresses 0x004E94A0 and 0x004E94D0. */
int32_t pharaoh_population_count_school_age(
    const int16_t people_at_age[PHARAOH_CENSUS_ACADEMY_AGE_END]);
int32_t pharaoh_population_count_academy_age(
    const int16_t people_at_age[PHARAOH_CENSUS_ACADEMY_AGE_END]);

void pharaoh_population_remove_from_census(
    int16_t people_at_age[PHARAOH_CENSUS_AGE_COUNT], int32_t people,
    const uint32_t random_pool[PHARAOH_CENSUS_AGE_COUNT],
    int32_t random_pool_index);
void pharaoh_city_population_remove_homeless(
    PharaohDisplacedPopulationState *state, int32_t people,
    const uint32_t random_pool[PHARAOH_CENSUS_AGE_COUNT],
    int32_t random_pool_index);
#endif
