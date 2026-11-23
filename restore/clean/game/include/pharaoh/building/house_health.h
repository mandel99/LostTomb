#ifndef PHARAOH_BUILDING_HOUSE_HEALTH_H
#define PHARAOH_BUILDING_HOUSE_HEALTH_H
#include "pharaoh/building/type_queries.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/runtime/random_generator.h"
#include "pharaoh/scenario/scenario_init_custom.h"

typedef struct PharaohHouseHealthContext {
    PharaohBuildingTypeRecord *buildings;
    const PharaohHouseModel *house_models;
    const PharaohScenarioPlayerRuntime *players;
    const uint32_t *current_player;
    const uint8_t *difficulty;
    PharaohRandomGeneratorContext random;
    PharaohMapRingActorSearch search;
} PharaohHouseHealthContext;

void pharaoh_house_update_health(PharaohHouseHealthContext *c,
    int32_t building,uint32_t recalculate); /*004E7F00*/
#endif
