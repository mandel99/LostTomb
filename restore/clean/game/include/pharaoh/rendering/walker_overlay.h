#ifndef PHARAOH_RENDERING_WALKER_OVERLAY_H
#define PHARAOH_RENDERING_WALKER_OVERLAY_H
#include "pharaoh/walker/record_queries.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/scenario/scenario_init_custom.h"
#include "pharaoh/rendering/minimap_walkers.h"

typedef struct PharaohWalkerOverlayContext {
    const int32_t *overlay,*current_player;
    const PharaohWalkerQueryRecord *walkers;
    const PharaohBuildingTypeRecord *buildings;
    const PharaohScenarioPlayerRuntime *players;
} PharaohWalkerOverlayContext;

int32_t pharaoh_walker_matches_overlay(const PharaohWalkerOverlayContext *,int32_t walker_id);
void pharaoh_minimap_bind_walker_overlay(PharaohMinimapWalkers *,PharaohWalkerOverlayContext *);
#endif
