#ifndef PHARAOH_SIMULATION_CULTURE_RATINGS_H
#define PHARAOH_SIMULATION_CULTURE_RATINGS_H

#include <stdint.h>

enum {
    PHARAOH_CULTURE_RATING_RECORD_COUNT = 11,
    PHARAOH_CULTURE_RATING_BAND_COUNT = 7
};

typedef struct PharaohCultureRatingRecord {
    int32_t coverage_index;
    int32_t points[PHARAOH_CULTURE_RATING_BAND_COUNT];
    int32_t limits[PHARAOH_CULTURE_RATING_BAND_COUNT - 1];
} PharaohCultureRatingRecord;

void pharaoh_city_ratings_culture_lowest(
    const PharaohCultureRatingRecord records[PHARAOH_CULTURE_RATING_RECORD_COUNT],
    const int32_t *coverage, int32_t *lowest_points, int32_t *lowest_index);

void pharaoh_city_ratings_culture_bonus(
    const PharaohCultureRatingRecord records[PHARAOH_CULTURE_RATING_RECORD_COUNT],
    const int32_t *coverage, int32_t lowest_points, int32_t *bonus);
void pharaoh_city_ratings_reduce_prosperity_after_bailout(
    int32_t *prosperity, int32_t *explanation);
void pharaoh_city_ratings_peace_explanation(
    int32_t condition_8, int32_t condition_7, int32_t condition_6,
    int32_t peace_rating, int32_t *explanation);
void pharaoh_city_ratings_kingdom_clear_yearly(
    int32_t *value_0, int32_t *value_1, int32_t *value_2,
    int32_t *value_3, int32_t *value_4, int32_t *value_5,
    int32_t *value_6);
void pharaoh_city_ratings_change_kingdom(int32_t *value, int32_t delta);

typedef struct PharaohCultureRatingContext {
    const PharaohCultureRatingRecord *records; /* eleven56-byte records5C9350 */
    const int32_t *coverage; /* actual78D5F0 array */
    int32_t *lowest_points,*lowest_index,*bonus; /* 5C9348/705738/705734 */
    int32_t *city_rating,*city_explanation; /* EA5B3C/EA5CA4 + player*49D8 */
} PharaohCultureRatingContext;

/* Complete004269B0, its native jump entry0046E1C0 and parent0046E1D0.
   Bind current-player fields and genuine shared scratch cells. Their children
   do not change the current player. No native code-pointer forwarding. */
void pharaoh_city_ratings_update_culture(const PharaohCultureRatingContext *);
void pharaoh_city_ratings_update_culture_thunk(
    const PharaohCultureRatingContext *);
void pharaoh_city_ratings_update_culture_advisor(
    const PharaohCultureRatingContext *);

typedef struct PharaohKingdomExplanationContext {
    /* Actual DWORD fields; addresses below are for player0. Their semantic
       names remain unqualified, so keep the native field identities. */
    const int32_t *value_ea450c,*value_ea5cbc;
    int32_t *value_ea5df0;
    const int32_t *value_ea5d5c,*value_ea5df8,*value_ea5d58;
    const int32_t *value_ea5df4,*value_ea5e00;
    int32_t *kingdom_explanation; /* EA5CB0 */
    const int32_t *peace_condition_8,*peace_condition_7,*peace_condition_6;
    const int32_t *peace_rating; /* EA5EA4/EA5EB4/EA5C9C/EA5B4C */
    int32_t *peace_explanation; /* EA5CB4 */
} PharaohKingdomExplanationContext;

/* Complete0046EC70, using the actual0046E880 C child and wrapped DWORD
   subtraction/increment. All pointers must bind the same actual player. */
void pharaoh_city_ratings_kingdom_explanation(
    const PharaohKingdomExplanationContext *);

#endif
