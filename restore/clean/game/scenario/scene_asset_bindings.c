#include "pharaoh/scenario/scene_asset_bindings.h"

static void building_models(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    c->building_models->difficulty = (PharaohModelDifficulty)c->settings->difficulty;
    (void)pharaoh_building_model_load(c->building_models);
}

static void figure_models(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    c->figure_models->difficulty = (PharaohModelDifficulty)c->settings->difficulty;
    (void)pharaoh_figure_model_load(c->figure_models);
}

static void tax_model(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    c->tax_model->difficulty = (PharaohModelDifficulty)c->settings->difficulty;
    (void)pharaoh_tax_sentiment_model_load(c->tax_model);
}

static void monument_scripts(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    pharaoh_scene_load_monument_libraries(c->monuments);
}

static void extra_scripts(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    pharaoh_scene_load_extra_library(c->monuments);
}

static void obelisk_scripts(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    pharaoh_scene_load_obelisk_libraries(c->monuments);
}

static void enemy_properties(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    pharaoh_scene_initialize_enemy_properties(c->enemies);
}

static void city_sound(void *user_data)
{
    PharaohSceneAssetBindings *const c = user_data;
    pharaoh_sound_city_initialize(c->sound);
}

void pharaoh_saved_game_bind_asset_steps(
    PharaohSavedGameInitializeOperations *operations,
    PharaohSceneAssetBindings *bindings)
{
    static const enum PharaohSavedGameInitializeStep steps[8] = {
        PHARAOH_INIT_BUILDING_MODEL, PHARAOH_INIT_FIGURE_MODEL,
        PHARAOH_INIT_TAX_MODEL, PHARAOH_INIT_MONUMENT_SCRIPTS,
        PHARAOH_INIT_EXTRA_SCRIPTS, PHARAOH_INIT_ENEMY_PROPERTIES,
        PHARAOH_INIT_CITY_SOUND, PHARAOH_INIT_OBELISK_SCRIPTS
    };
    static const PharaohSavedGameStep functions[8] = {
        building_models, figure_models, tax_model, monument_scripts,
        extra_scripts, enemy_properties, city_sound, obelisk_scripts
    };
    for (uint32_t i = 0; i < 8; ++i) {
        operations->step[steps[i]].user_data = bindings;
        operations->step[steps[i]].apply = functions[i];
    }
}
