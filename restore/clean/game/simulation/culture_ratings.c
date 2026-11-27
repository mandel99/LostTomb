#include "pharaoh/simulation/culture_ratings.h"
#include "pharaoh/ui/window_state_actions.h"

static int32_t signed_dword(uint32_t bits)
{
    return bits <= INT32_MAX ? (int32_t)bits :
        (int32_t)((int64_t)bits - INT64_C(0x100000000));
}

static int32_t add_dword(int32_t first, int32_t second)
{
    return signed_dword((uint32_t)first + (uint32_t)second);
}

_Static_assert(sizeof(PharaohCultureRatingRecord) == 56,
    "native culture-rating record size changed");

/* Complete C translation of Pharaoh.exe 0x00426A00. */
void pharaoh_city_ratings_culture_lowest(
    const PharaohCultureRatingRecord records[PHARAOH_CULTURE_RATING_RECORD_COUNT],
    const int32_t *coverage, int32_t *lowest_points, int32_t *lowest_index)
{
    int32_t index;

    int32_t lowest = 100;
    for (index = 0; index < PHARAOH_CULTURE_RATING_RECORD_COUNT; ++index) {
        const PharaohCultureRatingRecord *record = &records[index];
        const int32_t value = coverage[record->coverage_index];
        int32_t points;

        if (value <= record->limits[0]) points = record->points[0];
        else if (value < record->limits[1]) points = record->points[1];
        else if (value < record->limits[2]) points = record->points[2];
        else if (value < record->limits[3]) points = record->points[3];
        else if (value < record->limits[4]) points = record->points[4];
        else if (value < record->limits[5]) points = record->points[5];
        else points = record->points[6];

        if (points < lowest) {
            *lowest_index = index;
            lowest = points;
        }
    }
    *lowest_points = lowest;
}

/* Complete C translation of Pharaoh.exe 0x00426A80. */
void pharaoh_city_ratings_culture_bonus(
    const PharaohCultureRatingRecord records[PHARAOH_CULTURE_RATING_RECORD_COUNT],
    const int32_t *coverage, int32_t lowest_points, int32_t *bonus)
{
    static const int32_t upper_bounds[6] = {1, 20, 40, 60, 80, 100};
    int32_t index;

    int32_t total = 0;
    for (index = 0; index < PHARAOH_CULTURE_RATING_RECORD_COUNT; ++index) {
        const PharaohCultureRatingRecord *record = &records[index];
        const int32_t value = coverage[record->coverage_index];
        int32_t band = 0;

        while (band < 6 && value >= upper_bounds[band]) ++band;
        if (add_dword(record->points[band], 30) < lowest_points) total += 5;
    }
    *bonus = total;
}

/* Complete C translation of Pharaoh.exe 0x0046E840. */
void pharaoh_city_ratings_reduce_prosperity_after_bailout(
    int32_t *prosperity, int32_t *explanation)
{
    const uint32_t reduced = (uint32_t)*prosperity - 3u;
    *prosperity = signed_dword(reduced);
    if (*prosperity < 0) *prosperity = 0;
    *explanation = 8;
}

/* Complete C translation of Pharaoh.exe 0x0046E880. */
void pharaoh_city_ratings_peace_explanation(
    int32_t condition_8, int32_t condition_7, int32_t condition_6,
    int32_t peace_rating, int32_t *explanation)
{
    if (condition_8 != 0) *explanation = 8;
    else if (condition_7 != 0) *explanation = 7;
    else if (condition_6 != 0) *explanation = 6;
    else if (peace_rating < 10) *explanation = 0;
    else if (peace_rating < 30) *explanation = 1;
    else if (peace_rating < 60) *explanation = 2;
    else if (peace_rating < 90) *explanation = 3;
    else if (peace_rating < 100) *explanation = 4;
    else *explanation = 5;
}

/* Complete C translation of Pharaoh.exe 0x0046E930. */
void pharaoh_city_ratings_kingdom_clear_yearly(
    int32_t *value_0, int32_t *value_1, int32_t *value_2,
    int32_t *value_3, int32_t *value_4, int32_t *value_5,
    int32_t *value_6)
{
    *value_0 = 0;
    *value_1 = 0;
    *value_2 = 0;
    *value_3 = 0;
    *value_4 = 0;
    *value_5 = 0;
    *value_6 = 0;
}

/* Complete C translation of Pharaoh.exe 0x004E96C0. */
void pharaoh_city_ratings_change_kingdom(int32_t *value, int32_t delta)
{
    *value = add_dword(*value, delta);
    if (*value < 0) *value = 0;
    if (*value > 100) *value = 100;
}

void pharaoh_city_ratings_update_culture(const PharaohCultureRatingContext *c)
{
    pharaoh_city_ratings_culture_lowest(c->records, c->coverage,
        c->lowest_points, c->lowest_index);
    pharaoh_city_ratings_culture_bonus(c->records, c->coverage,
        *c->lowest_points, c->bonus);
    int32_t rating = add_dword(*c->lowest_points, *c->bonus);
    if (rating >= 100) rating = 100;
    else if (rating < 0) rating = 0;
    const int32_t lowest_index = *c->lowest_index;
    *c->city_rating = rating;
    *c->city_explanation = lowest_index;
}

void pharaoh_city_ratings_update_culture_thunk(
    const PharaohCultureRatingContext *c)
{
    pharaoh_city_ratings_update_culture(c);
}

void pharaoh_city_ratings_update_culture_advisor(
    const PharaohCultureRatingContext *c)
{
    pharaoh_city_ratings_update_culture_thunk(c);
    pharaoh_city_ratings_store_culture_lowest(c->city_explanation,
        *c->lowest_index);
}

void pharaoh_city_ratings_kingdom_explanation(
    const PharaohKingdomExplanationContext *c)
{
    *c->value_ea5df0 = 0;
    const int32_t subtrahend = *c->value_ea5cbc;
    int32_t difference = signed_dword(
        (uint32_t)*c->value_ea450c - (uint32_t)subtrahend);
    if (difference > 0) {
        if (subtrahend != 0) difference = add_dword(difference, 1);
        *c->value_ea5df0 = difference;
    }
    pharaoh_city_ratings_peace_explanation(*c->peace_condition_8,
        *c->peace_condition_7, *c->peace_condition_6,
        *c->peace_rating, c->peace_explanation);
    difference = *c->value_ea5df0;
    int32_t reason;
    if (difference >= 8) reason = 1;
    else {
        const int32_t first = *c->value_ea5d5c;
        if (first >= 3) reason = 2;
        else {
            const int32_t second = *c->value_ea5df8;
            if (second >= 5) reason = 3;
            else if (difference >= 5) reason = 4;
            else if (first >= 2) reason = 5;
            else if (second >= 3) reason = 6;
            else if (difference >= 3) reason = 7;
            else if (*c->value_ea5d58 != 0) reason = 8;
            else if (difference >= 2) reason = 9;
            else if (*c->value_ea5df4 != 0) reason = 10;
            else if (difference != 0) reason = 11;
            else {
                const int32_t last = *c->value_ea5e00;
                if (last == 2) reason = 12;
                else if (last == 1) reason = 13;
                else reason = 0;
            }
        }
    }
    *c->kingdom_explanation = reason;
}
