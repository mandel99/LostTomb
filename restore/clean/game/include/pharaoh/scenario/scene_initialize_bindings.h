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
#include "pharaoh/map/rotated_buildings.h"
#include "pharaoh/commerce/export_keep.h"
#include "pharaoh/simulation/flood_build.h"

/* All contexts and live scene buffers remain caller-owned. All45 direct
 * initialization slots have C owners. Every deeper operation must also have
 * a real provider before host attachment. */
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
    PharaohRotatedBuildingContext *rotated;
    PharaohExportKeepContext *export_keep;
    PharaohFloodBuildContext *floods;
    PharaohFloodListsContext *flood_lists;
    PharaohFloodBuildOperations *flood_operations;
} PharaohSceneInitializeBindings;

void pharaoh_saved_game_bind_scene(
    PharaohSavedGameInitializeOperations *operations,
    PharaohSceneInitializeBindings *bindings);

#endif
