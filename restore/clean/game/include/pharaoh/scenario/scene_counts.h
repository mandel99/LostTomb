#ifndef PHARAOH_SCENARIO_SCENE_COUNTS_H
#define PHARAOH_SCENARIO_SCENE_COUNTS_H

#include "pharaoh/map/road_network.h"
#include "pharaoh/map/road_access.h"
#include "pharaoh/building/occupancy_query.h"
#include "pharaoh/simulation/industry_queries.h"

typedef struct PharaohCityBuildingCounts {
    uint32_t total[237], active[237];
    uint32_t services_total[36], services_active[36];
} PharaohCityBuildingCounts;

typedef struct PharaohSceneCounts {
    PharaohSavedGameInitializeContext *initialize;
    PharaohRoadNetworkContext *roads;
    PharaohRoadAccessContext *access;
    PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    const int32_t *highest_building;
    PharaohCityBuildingCounts *counts;
    PharaohOccupancyState *occupancy;
    PharaohIndustryQueryContext farm_query;
} PharaohSceneCounts;

void pharaoh_city_rebuild_building_counts(PharaohSceneCounts *c);
void pharaoh_saved_game_bind_count_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneCounts *c);

#endif
