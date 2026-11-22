#ifndef PHARAOH_SCENARIO_SCENE_ASSET_BINDINGS_H
#define PHARAOH_SCENARIO_SCENE_ASSET_BINDINGS_H

#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/scenario/scene_assets.h"
#include "pharaoh/runtime/city_sound.h"
#include "pharaoh/runtime/building_model_loading.h"
#include "pharaoh/runtime/figure_model_loading.h"
#include "pharaoh/runtime/tax_sentiment_model.h"

typedef struct PharaohSceneAssetBindings {
    PharaohSettingsFile *settings;
    PharaohBuildingModelLoadContext *building_models;
    PharaohFigureModelLoadContext *figure_models;
    PharaohTaxSentimentModelLoadContext *tax_model;
    PharaohSceneMonumentAssets *monuments;
    PharaohSceneEnemyAssets *enemies;
    PharaohCitySoundInitializeContext *sound;
} PharaohSceneAssetBindings;

/* The caller keeps these contexts alive. Other initialization slots retain
 * their own C context bindings; no missing operation is replaced with a no-op. */
void pharaoh_saved_game_bind_asset_steps(
    PharaohSavedGameInitializeOperations *operations,
    PharaohSceneAssetBindings *bindings);

#endif
