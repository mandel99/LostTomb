#ifndef PHARAOH_SCENARIO_SCENE_INITIALIZE_BINDINGS_H
#define PHARAOH_SCENARIO_SCENE_INITIALIZE_BINDINGS_H

#include "pharaoh/scenario/scene_asset_bindings.h"
#include "pharaoh/scenario/scene_records.h"
#include "pharaoh/scenario/scene_routing.h"
#include "pharaoh/scenario/scene_counts.h"
#include "pharaoh/scenario/scene_ui.h"
#include "pharaoh/scenario/scene_storage.h"
#include "pharaoh/scenario/scene_surface.h"
#include "pharaoh/scenario/scene_economy.h"
#include "pharaoh/building/kingdom_access.h"
#include "pharaoh/commerce/export_keep.h"
#include "pharaoh/simulation/flood_build.h"

/* All contexts and live scene buffers remain caller-owned. The rotated-image
 * slot still requires a verified C binding from the caller; the pending
 * rotated_buildings unit and its image dependencies are not canonical yet.
 * Every deeper operation must have a real provider before host attachment. */
typedef struct PharaohSceneInitializeBindings {
    PharaohSavedGameInitializeContext *initialize;
    PharaohSceneAssetBindings *assets;
    PharaohSceneRecords *records;
    PharaohSceneRouting *routing;
    PharaohSceneCounts *counts;
    PharaohSceneUi *ui;
    PharaohSceneStorage *storage;
    PharaohSceneEconomy *economy;
    PharaohMapSurfaceContext *surface;
    PharaohBuildingAccessContext *kingdom;
    PharaohSavedGameStepBinding rotated;
    PharaohExportKeepContext *export_keep;
    PharaohFloodBuildContext *floods;
    PharaohFloodListsContext *flood_lists;
    PharaohFloodBuildOperations *flood_operations;
} PharaohSceneInitializeBindings;

void pharaoh_saved_game_bind_scene(
    PharaohSavedGameInitializeOperations *operations,
    PharaohSceneInitializeBindings *bindings);

#endif
