#ifndef PHARAOH_RENDERING_BUILDING_ANIMATION_OFFSET_H
#define PHARAOH_RENDERING_BUILDING_ANIMATION_OFFSET_H

#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/building/model_types.h"
#include "pharaoh/building/type_queries.h"

typedef struct PharaohBuildingAnimationOffsetContext {
    const int16_t *building_ids;
    uint8_t *sprites;
    const PharaohBuildingTypeRecord *buildings;
    const PharaohBuildingModelRecord *models;
    const PharaohSpriteImageRecord *images;
    int32_t image_count;
    const uint8_t *animation_should_advance;
    int32_t cheat_allow_all;
    void *user_data;
    int32_t (*is_floodplain_farm)(void *,int32_t building);
    int32_t (*houses_water_in_radius)(void *,int32_t building,int32_t radius);
    int32_t (*is_palace)(void *,int32_t building);
    int32_t (*main_building)(void *,int32_t building);
} PharaohBuildingAnimationOffsetContext;

int32_t pharaoh_building_animation_offset(
    PharaohBuildingAnimationOffsetContext *context,
    int32_t image,int32_t tile); /*0040F2C0*/

#endif
