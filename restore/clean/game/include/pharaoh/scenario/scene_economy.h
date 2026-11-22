#ifndef PHARAOH_SCENARIO_SCENE_ECONOMY_H
#define PHARAOH_SCENARIO_SCENE_ECONOMY_H

#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/commerce/resource_supply.h"
#include "pharaoh/commerce/empire_images.h"

typedef struct PharaohSceneEconomy {
    const PharaohResourceAvailabilityContext *resources;
    const PharaohEmpireImagesContext *images;
} PharaohSceneEconomy;

void pharaoh_saved_game_bind_economy_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneEconomy *c);

#endif
