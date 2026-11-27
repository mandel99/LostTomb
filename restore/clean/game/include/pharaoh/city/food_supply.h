#ifndef PHARAOH_CITY_FOOD_SUPPLY_H
#define PHARAOH_CITY_FOOD_SUPPLY_H
#include "pharaoh/city/food_rate.h"
#include "pharaoh/scenario/tutorial_complete.h"
#include "pharaoh/map/road_access.h"

typedef struct PharaohCityFoodContext {
    PharaohBuildingTypeRecord *buildings;
    const PharaohBuildingModelRecord *models; /*705EA4,40-byte records*/
    const int32_t *building_count,*population;
    const uint32_t *editor_active; /*784BBC*/
    uint32_t *stocks; /*9 DWORDs EA60AC + player*49D8*/
    uint32_t *total_stored,*available_types,*months,*staffed_granaries;
    uint32_t *under_100,*under_50,*under_50_with_food;
    int32_t *monthly_need;
    PharaohRoadAccessContext *road;
    const PharaohFoodRateContext *rate;
    const PharaohTutorialCompletionContext *tutorial;
} PharaohCityFoodContext;

typedef struct PharaohCityFoodUpdateContext {
    const uint8_t *enabled; /*EA624A*/
    const PharaohCityFoodContext *player_one; /*native calls rebuild(1)*/
    PharaohBuildingTypeRecord *buildings;
    const int32_t *building_count;
    const uint32_t *editor_active;
} PharaohCityFoodUpdateContext;

/* Complete0045D630/0045D5D0. Every player slot is the actual requested bank;
   the food-rate context retains its separate selected-player temple check. */
void pharaoh_city_rebuild_food(const PharaohCityFoodContext *);
void pharaoh_city_update_food(const PharaohCityFoodUpdateContext *);
#endif
