#ifndef PHARAOH_UI_WALKER_PORTRAIT_H
#define PHARAOH_UI_WALKER_PORTRAIT_H

#include <stdint.h>

typedef struct PharaohBuildingTypeRecord PharaohBuildingTypeRecord;
typedef struct PharaohWalkerQueryRecord PharaohWalkerQueryRecord;

typedef void (*PharaohBuildingInfoDrawSprite)(void *user_data,
    int32_t image_id, int32_t x, int32_t y);

typedef struct PharaohWalkerPortraitImages {
    int32_t big_people;
    int32_t big_people_2;
    int32_t big_people_3;
    int32_t big_people_4;
} PharaohWalkerPortraitImages;

typedef struct PharaohClimatePreyRecord {
    int32_t walker_type;
    uint8_t reserved_04[0x2c];
} PharaohClimatePreyRecord;

int32_t pharaoh_building_info_walker_big_image(
    const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings,
    const PharaohClimatePreyRecord climate_prey[3],
    const PharaohWalkerPortraitImages *images,
    int32_t scenario_climate, int32_t walker_id);

void pharaoh_building_info_draw_resource_icon(
    int32_t resource_index, int32_t x, int32_t y,
    int32_t resource_sprite_base, int32_t *text_width,
    PharaohBuildingInfoDrawSprite draw_sprite, void *user_data);

void pharaoh_building_info_draw_carried_goods(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id,
    int32_t x, int32_t y, int32_t resource_sprite_base,
    int32_t *text_width, PharaohBuildingInfoDrawSprite draw_sprite,
    void *user_data);

void pharaoh_building_info_draw_walker_goods(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id,
    int32_t x, int32_t y, int32_t resource_sprite_base,
    int32_t *text_width, PharaohBuildingInfoDrawSprite draw_sprite,
    void *user_data);

#endif
