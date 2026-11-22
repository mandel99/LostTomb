#include "pharaoh/map/moisture.h"
static const double fertility_scale=96.0;

/* Native00486EC0: two integer arguments, not an invented live-ST0 API. */
int32_t pharaoh_map_fertility_at_distance(int32_t distance,int32_t limit)
{
    const uint32_t difference=(uint32_t)limit-(uint32_t)distance;
    const uint32_t positive=(int32_t)difference<0?0:difference;
    const int32_t numerator=limit<(int32_t)positive?limit:(int32_t)positive;
    const int32_t denominator=limit<12?12:limit;
    return (int32_t)((uint32_t)pharaoh_x87_scaled_integer_ratio(
        numerator,denominator,&fertility_scale)-1u);
}
