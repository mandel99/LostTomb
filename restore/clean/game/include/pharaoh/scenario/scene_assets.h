#ifndef PHARAOH_SCENARIO_SCENE_ASSETS_H
#define PHARAOH_SCENARIO_SCENE_ASSETS_H

#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/monuments/definitions.h"
#include "pharaoh/walker/model_types.h"

typedef struct PharaohSceneMonumentAssets {
    const PharaohSpriteLibraryLoadContext *libraries;
    const int16_t *goal_definition_ids;
    uint8_t *animation_set;
    const int8_t *mission_id;
    const char *scenario_name;
    int32_t *pyramid_style;
    int32_t *extra_script_monument;
    int32_t *mausoleum_size;
} PharaohSceneMonumentAssets;

typedef struct PharaohSceneEnemyAssets {
    const PharaohSpriteLibraryLoadContext *libraries;
    int16_t *enemy_type;
    const PharaohEnemyModelTable *enemy_models;
    PharaohFigureModelTable *figure_models;
} PharaohSceneEnemyAssets;

/* Native004F4520,0053D970,004BC720; completed C providers reused from the
 * prior MissionMapLoader. These call the actual C sprite-library loader. */
void pharaoh_scene_load_monument_libraries(const PharaohSceneMonumentAssets *c);
void pharaoh_scene_load_extra_library(const PharaohSceneMonumentAssets *c);
void pharaoh_scene_initialize_enemy_properties(const PharaohSceneEnemyAssets *c);

#endif
