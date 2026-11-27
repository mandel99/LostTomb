#include "pharaoh/city/advisor_reasons.h"

void pharaoh_city_ratings_advisor_reasons(PharaohAdvisorReasonState *s)
{
    int32_t largest;
    switch ((*s->sentiment_low_mood_cause)) {
        case 1: (*s->no_immigration_cause) = 2; break;
        case 2: (*s->no_immigration_cause) = 1; break;
        case 3: (*s->no_immigration_cause) = 3; break;
        case 4: (*s->no_immigration_cause) = 0; break;
        case 5: (*s->no_immigration_cause) = 4; break;
        default: (*s->no_immigration_cause) = 5; break;
    }
    (*s->health_demand) = 0;
    largest = 0;
    if ((*s->houses_missing_courthouse) > 0 ||
        (*s->houses_missing_any_courthouse) > 0) {
        if ((*s->houses_missing_courthouse) < (*s->houses_missing_any_courthouse)) {
            (*s->health_demand) = 4;
            largest = (*s->houses_missing_any_courthouse);
        } else {
            (*s->health_demand) = 1;
            largest = (*s->houses_missing_courthouse);
        }
    }
    if (largest < (*s->houses_missing_dentist)) {
        (*s->health_demand) = 2;
        largest = (*s->houses_missing_dentist);
    }
    if (largest < (*s->houses_missing_health)) (*s->health_demand) = 3;

    (*s->education_demand) = 0;
    if ((*s->houses_missing_education) < (*s->houses_missing_more_education))
        (*s->education_demand) = 1;
    else if ((*s->houses_missing_more_education) < (*s->houses_missing_education))
        (*s->education_demand) = 2;
    else if ((*s->houses_missing_more_education) != 0 ||
             (*s->houses_missing_education) != 0)
        (*s->education_demand) = 3;

    (*s->entertainment_demand) = 0;
    if ((*s->houses_missing_more_entertainment) < (*s->houses_missing_entertainment))
        (*s->entertainment_demand) = 1;
    else if ((*s->houses_missing_more_entertainment) != 0)
        (*s->entertainment_demand) = 2;

    (*s->religion_demand) = 0;
    largest = 0;
    if ((*s->houses_missing_religion) > 0) {
        (*s->religion_demand) = 1;
        largest = (*s->houses_missing_religion);
    }
    if (largest < (*s->houses_missing_second_religion)) {
        (*s->religion_demand) = 2;
        largest = (*s->houses_missing_second_religion);
    }
    if (largest < (*s->houses_missing_third_religion)) (*s->religion_demand) = 3;
}
