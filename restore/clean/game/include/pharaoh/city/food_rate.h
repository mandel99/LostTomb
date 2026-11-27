#ifndef PHARAOH_CITY_FOOD_RATE_H
#define PHARAOH_CITY_FOOD_RATE_H
#include "pharaoh/city/prosperity.h"

typedef struct PharaohFoodRateContext {
    const PharaohTempleWageBonusContext *requested_city;
    const PharaohTempleWageBonusContext *selected_city;
} PharaohFoodRateContext;

/* Complete004C5340. The second temple check uses the actual selected city,
   independently of the city passed to the first check. */
int32_t pharaoh_city_food_consumption_rate(const PharaohFoodRateContext *);
#endif
