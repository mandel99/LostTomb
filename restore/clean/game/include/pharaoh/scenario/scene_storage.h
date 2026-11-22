#ifndef PHARAOH_SCENARIO_SCENE_STORAGE_H
#define PHARAOH_SCENARIO_SCENE_STORAGE_H

#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/scenario/scene_industry.h"
#include "pharaoh/scenario/scene_assets.h"
#include "pharaoh/commerce/granary_supply.h"
#include "pharaoh/commerce/granary_repair.h"
#include "pharaoh/monuments/scenario_slots.h"

typedef struct PharaohSceneStorage {
    PharaohSavedGameInitializeContext *initialize;
    PharaohBuildingTypeRecord *buildings;
    PharaohBuildingStorageRecord *storages;
    PharaohGranarySupplyContext *granary_supply;
    PharaohMonumentSlotContext *monuments;
    const PharaohSceneMonumentAssets *asset_monuments;
} PharaohSceneStorage;

void pharaoh_saved_game_bind_storage_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneStorage *c);

#endif
