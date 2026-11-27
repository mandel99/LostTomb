#ifndef PHARAOH_CITY_ADVISOR_REASONS_H
#define PHARAOH_CITY_ADVISOR_REASONS_H

#include <stdint.h>

/* Borrow selected49D8 bank. Inputs: EA5DB0; EA5B98/EA5FBC/EA5B94/EA5BAC;
   more/regular education EA5B80/EA5B84, entertainment EA5B7C/EA5BF8;
   religion EA5BC0/BC4/BC8. Outputs EA5C80/8C/94/98/90 respectively. */
typedef struct PharaohAdvisorReasonState {
    const int32_t *sentiment_low_mood_cause;
    volatile int32_t *no_immigration_cause;
    volatile int32_t *health_demand;
    volatile int32_t *education_demand;
    volatile int32_t *entertainment_demand;
    volatile int32_t *religion_demand;
    const int32_t *houses_missing_courthouse;
    const int32_t *houses_missing_any_courthouse;
    const int32_t *houses_missing_dentist;
    const int32_t *houses_missing_health;
    const int32_t *houses_missing_more_education;
    const int32_t *houses_missing_education;
    const int32_t *houses_missing_more_entertainment;
    const int32_t *houses_missing_entertainment;
    const int32_t *houses_missing_religion;
    const int32_t *houses_missing_second_religion;
    const int32_t *houses_missing_third_religion;
} PharaohAdvisorReasonState;

void pharaoh_city_ratings_advisor_reasons(PharaohAdvisorReasonState *state);

#endif
