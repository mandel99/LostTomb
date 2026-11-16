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
