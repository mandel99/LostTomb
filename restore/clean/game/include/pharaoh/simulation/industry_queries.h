#ifndef PHARAOH_SIMULATION_INDUSTRY_QUERIES_H
#define PHARAOH_SIMULATION_INDUSTRY_QUERIES_H

#include <stdbool.h>
#include <stdint.h>

typedef bool (*PharaohBuildingPredicate)(void *user_data, int32_t building_id);
typedef int32_t (*PharaohBuildingTypeFunction)(void *user_data, int32_t building_id);
typedef int32_t (*PharaohFarmCropFunction)(void *user_data, int32_t building_type);
typedef void (*PharaohRawProducerDrawFunction)(void *user_data, int32_t help,
    int32_t resource, int32_t language_group, int32_t shows_stock);

typedef struct PharaohIndustryQueryContext {
    int32_t current_month;
    int32_t flood_season_month;
    int32_t quarry_help;
    const int32_t *farm_harvest_months;
    PharaohBuildingPredicate is_farm;
    PharaohBuildingPredicate is_on_floodplain;
    PharaohBuildingTypeFunction building_type;
    PharaohFarmCropFunction crop_for_type;
    PharaohRawProducerDrawFunction draw_raw_producer;
    void *user_data;
} PharaohIndustryQueryContext;

int32_t pharaoh_farm_next_harvest_month(
    const PharaohIndustryQueryContext *context, int32_t building_id);
int32_t pharaoh_building_is_floodplain_farm(
    const PharaohIndustryQueryContext *context, int32_t building_id);
void pharaoh_draw_quarry(
    const PharaohIndustryQueryContext *context, int32_t resource);

#endif
