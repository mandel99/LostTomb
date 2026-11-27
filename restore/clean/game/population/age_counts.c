#include "pharaoh/population/age_counts.h"

enum { PHARAOH_CENSUS_SCHOOL_AGE_END = 14 };

int32_t pharaoh_population_count_school_age(
    const int16_t people_at_age[PHARAOH_CENSUS_ACADEMY_AGE_END])
{
    int32_t age;
    int32_t count = 0;

    for (age = 0; age < PHARAOH_CENSUS_SCHOOL_AGE_END; ++age) {
        count += people_at_age[age];
    }
    return count;
}

int32_t pharaoh_population_count_academy_age(
    const int16_t people_at_age[PHARAOH_CENSUS_ACADEMY_AGE_END])
{
    int32_t age;
    int32_t count = 0;

    for (age = PHARAOH_CENSUS_SCHOOL_AGE_END;
         age < PHARAOH_CENSUS_ACADEMY_AGE_END;
         ++age) {
        count += people_at_age[age];
    }
    return count;
}

void pharaoh_population_remove_from_census(
    int16_t people_at_age[PHARAOH_CENSUS_AGE_COUNT], int32_t people,
    const uint32_t random_pool[PHARAOH_CENSUS_AGE_COUNT],
    int32_t random_pool_index)
{
    int32_t cursor = random_pool_index;
    int32_t misses = 0;
    uint32_t remaining = (uint32_t)people;
    while (remaining != 0u && misses < PHARAOH_CENSUS_AGE_COUNT) {
        const int32_t age = (int32_t)(random_pool[cursor] & 0x3fu);
        if (people_at_age[age] > 0) {
            --people_at_age[age];
            --remaining;
            misses = 0;
        } else {
            ++misses;
        }
        if (++cursor >= PHARAOH_CENSUS_AGE_COUNT) cursor = 0;
    }
    cursor = 10;
    misses = 0;
    while (remaining != 0u && misses < PHARAOH_CENSUS_AGE_COUNT) {
        if (people_at_age[cursor] > 0) {
            --people_at_age[cursor];
            --remaining;
            misses = 0;
        } else {
            ++misses;
        }
        if (++cursor >= PHARAOH_CENSUS_AGE_COUNT) cursor = 0;
    }
}

void pharaoh_city_population_remove_homeless(
    PharaohDisplacedPopulationState *state, int32_t people,
    const uint32_t random_pool[PHARAOH_CENSUS_AGE_COUNT],
    int32_t random_pool_index)
{
    int32_t age;
    *state->displaced_population = (int32_t)(
        (uint32_t)*state->displaced_population + (uint32_t)people);
    pharaoh_population_remove_from_census(
        state->people_at_age, people, random_pool, random_pool_index);
    *state->population = 0;
    for (age = 0; age < PHARAOH_CENSUS_AGE_COUNT; ++age) {
        *state->population = (int32_t)((uint32_t)*state->population +
            (uint32_t)(int32_t)state->people_at_age[age]);
    }
    if (*state->highest_population < *state->population)
        *state->highest_population = *state->population;
}
