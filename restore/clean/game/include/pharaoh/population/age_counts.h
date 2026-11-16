#ifndef PHARAOH_POPULATION_AGE_COUNTS_H
#define PHARAOH_POPULATION_AGE_COUNTS_H

#include <stdint.h>

enum { PHARAOH_CENSUS_ACADEMY_AGE_END = 21 };

/* Original addresses 0x004E94A0 and 0x004E94D0. */
int32_t pharaoh_population_count_school_age(
    const int16_t people_at_age[PHARAOH_CENSUS_ACADEMY_AGE_END]);
int32_t pharaoh_population_count_academy_age(
    const int16_t people_at_age[PHARAOH_CENSUS_ACADEMY_AGE_END]);

#endif
