#include "pharaoh/city/food_rate.h"

int32_t pharaoh_city_food_consumption_rate(const PharaohFoodRateContext *c)
{
    const PharaohTempleWageBonusContext *city=c->requested_city;
    if(*city->temple_count!=0 &&
        pharaoh_building_group_index(city->buildings,*city->temple_building)==4)
        return 20;
    city=c->selected_city;
    if(*city->temple_count!=0 &&
        pharaoh_building_group_index(city->buildings,*city->temple_building)==0 &&
        pharaoh_temple_upgrade_present(city->buildings,*city->temple_building,1))
        return 20;
    return 25;
}
