#include "pharaoh/ui/walker_portrait.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/walker/record_queries.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohClimatePreyRecord) == 0x30,
    "native climate prey record layout");

static int32_t priest_portrait(const PharaohBuildingTypeRecord *buildings,
    int32_t building_id, int32_t fallback_type,
    const PharaohWalkerPortraitImages *images)
{
    switch (pharaoh_building_group_index(buildings, building_id)) {
    case 1: return images->big_people_3 + 3;
    case 2: return images->big_people_3 + 2;
    case 3: return images->big_people_3 + 4;
    case 4: return images->big_people_3 + 1;
    default: return images->big_people + fallback_type;
    }
}

/* Complete one-callee C translation of Pharaoh.exe 0x004FA8C0. */
int32_t pharaoh_building_info_walker_big_image(
    const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings,
    const PharaohClimatePreyRecord climate_prey[3],
    const PharaohWalkerPortraitImages *images,
    int32_t scenario_climate, int32_t walker_id)
{
    const PharaohWalkerQueryRecord *const walker = &walkers[walker_id];
    const int32_t type = walker->type;

    switch (type) {
    case 0x18: return images->big_people_4 + 3;
    case 0x1b:
        return priest_portrait(buildings, walker->building_id, type, images);
    case 0x1e: return images->big_people + 0x1d;
    case 0x49:
        switch (climate_prey[(uint8_t)scenario_climate].walker_type) {
        case 0x44: return images->big_people_3;
        case 0x45: return images->big_people_2;
        case 0x46: return images->big_people + type;
        default: return images->big_people;
        }
    case 0x5b: {
        const int32_t fallback_type = walker->counting_action_00b;
        if (fallback_type != 0x1b)
            return images->big_people + fallback_type;
        return priest_portrait(
            buildings, walker->building_id, fallback_type, images);
    }
    case 0x66: return images->big_people_4 + 4;
    case 0x67: return images->big_people_4 + 5;
    case 0x68: return images->big_people_4 + 2;
    case 0x69: return images->big_people_4;
    case 0x6a: return images->big_people_4 + 6;
    case 0x6b: return images->big_people_4 + 7;
    case 0x6c: return images->big_people_4 + 1;
    case 0x6d: return images->big_people_4 + 8;
    default: return images->big_people + type;
    }
}

/* Complete one-callee C translation of Pharaoh.exe 0x004FBF00. */
void pharaoh_building_info_draw_resource_icon(
    int32_t resource_index, int32_t x, int32_t y,
    int32_t resource_sprite_base, int32_t *text_width,
    PharaohBuildingInfoDrawSprite draw_sprite, void *user_data)
{
    const int32_t saved_text_width = *text_width;
    draw_sprite(user_data, resource_sprite_base + resource_index, x, y);
    *text_width = saved_text_width;
}

static int32_t carried_good_resource(int32_t inventory_slot)
{
    switch (inventory_slot) {
    case 0: return 1;
    case 1: return 2;
    case 2: return 3;
    case 3: return 4;
    case 4: return 5;
    case 5: return 6;
    case 6: return 7;
    case 7: return 8;
    case 8: return 15;
    case 9: return 13;
    case 10: return 17;
    case 11: return 19;
    default: return -1;
    }
}

/* Complete one-callee C translation of Pharaoh.exe 0x004FBF30. */
void pharaoh_building_info_draw_carried_goods(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id,
    int32_t x, int32_t y, int32_t resource_sprite_base,
    int32_t *text_width, PharaohBuildingInfoDrawSprite draw_sprite,
    void *user_data)
{
    const int32_t saved_text_width = *text_width;
    const PharaohWalkerQueryRecord *const walker = &walkers[walker_id];

    for (int32_t index = 0; index < 4; ++index) {
        const int32_t inventory_slot = walker->carried_goods[index];
        if (inventory_slot != -1) {
            *text_width = saved_text_width;
            draw_sprite(user_data,
                resource_sprite_base + carried_good_resource(inventory_slot),
                x, y);
            x += 0x20;
        }
    }
    *text_width = saved_text_width;
}

/* Complete one-callee C translation of Pharaoh.exe 0x004FC040. */
void pharaoh_building_info_draw_walker_goods(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id,
    int32_t x, int32_t y, int32_t resource_sprite_base,
    int32_t *text_width, PharaohBuildingInfoDrawSprite draw_sprite,
    void *user_data)
{
    const int32_t saved_text_width = *text_width;
    const PharaohWalkerQueryRecord *const walker = &walkers[walker_id];

    for (int32_t index = 0; index < 4; ++index) {
        const int32_t inventory_slot = walker->carried_goods[index];
        if (inventory_slot != -1 && walker->buyer_amounts[index] > 0) {
            *text_width = saved_text_width;
            draw_sprite(user_data,
                resource_sprite_base + carried_good_resource(inventory_slot),
                x, y);
            x += 0x20;
        }
    }
    *text_width = saved_text_width;
}
