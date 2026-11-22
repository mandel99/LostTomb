#ifndef PHARAOH_SCENARIO_SCENE_UI_H
#define PHARAOH_SCENARIO_SCENE_UI_H

#include "pharaoh/scenario/scene_counts.h"
#include "pharaoh/ui/build_menu.h"
#include "pharaoh/population/health_coverage.h"

typedef struct PharaohCityServiceCoverage {
    int32_t entertainment[5];
    int32_t school, academy, religion;
    int32_t health_75, health_76, health_6e, health_78;
    int32_t gods[5];
} PharaohCityServiceCoverage;

typedef struct PharaohSceneUi {
    PharaohBuildMenuContext *build_menu;
    const PharaohCityBuildingCounts *counts;
    const int16_t *god_status;
    const int16_t *people_at_age;
    const PharaohHealthCoverageBuilding *health_buildings;
    PharaohCityServiceCoverage *coverage;
} PharaohSceneUi;

void pharaoh_city_update_service_coverage(PharaohSceneUi *c);
void pharaoh_saved_game_bind_ui_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneUi *c);

#endif
