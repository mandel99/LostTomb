#include "pharaoh/rendering/minimap_native_metrics.h"

#include <string.h>

static int16_t word_at(const PharaohBuildingTypeRecord *building, size_t offset)
{
    int16_t value;
    memcpy(&value, (const uint8_t *)building + offset, sizeof(value));
    return value;
}

static bool farm_face_is_visible(const PharaohMinimapNativeMetrics *context,
    int32_t building_id)
{
    uint8_t expected;
    if (!pharaoh_building_is_farm(context->buildings, building_id)) return true;
    switch (*context->view_orientation) {
    case 0: expected = 0x50; break;
    case 2: expected = 0x40; break;
    case 4: expected = 0x42; break;
    case 6: expected = 0x52; break;
    default: return true;
    }
    return context->map_edge[*context->current_map_cell] == expected;
}

/* Complete C translation of Pharaoh.exe 0x00433C90. */
int32_t pharaoh_minimap_fire_risk_height(
    const PharaohMinimapNativeMetrics *context, int32_t building_id)
{
    const PharaohBuildingTypeRecord *building = &context->buildings[building_id];
    if (building->building_type == 0xa7 || building->building_type == 99 ||
        !farm_face_is_visible(context, building_id)) return 0;
    return word_at(building, 0x48) / 100;
}

/* Complete C translation of Pharaoh.exe 0x00433D50. */
int32_t pharaoh_minimap_damage_risk_height(
    const PharaohMinimapNativeMetrics *context, int32_t building_id)
{
    const PharaohBuildingTypeRecord *building = &context->buildings[building_id];
    *context->active_sprite_image = context->map_image[*context->current_map_cell];
    if (building->building_type == 0x51 ||
        !farm_face_is_visible(context, building_id)) return 0;
    return word_at(building, 0x46) / 100;
}

/* Complete C translation of Pharaoh.exe 0x004347B0. */
int32_t pharaoh_minimap_tax_income_height(
    const PharaohMinimapNativeMetrics *context, int32_t building_id)
{
    const PharaohBuildingTypeRecord *building = &context->buildings[building_id];
    int32_t total = 0;
    if (building->house_size == 0) return 0;
    for (int32_t index = 0; index < 5; ++index)
        total += context->active_tax_building_counts[index];
    if (total == 0) return 0;
    return pharaoh_building_house_tax_income(context->buildings, building_id) / 25;
}

static bool delivery_walker_is_active(const PharaohMinimapNativeMetrics *context,
    int32_t walker_id)
{
    const PharaohWalkerQueryRecord *walker;
    if (walker_id == 0) return false;
    walker = &context->walkers[walker_id];
    return walker->type == 4 && walker->action_state == 8 &&
        walker->reserved_066[0] != 0;
}

static bool type_uses_idle_delivery_rule(int32_t type)
{
    switch (type) {
    case 0x20: case 0x2f: case 0x33: case 0x35: case 0x4a: case 0x71:
    case 0xb1: case 0xb2: case 0xbb: case 0xbc: case 0xbd: case 0xc2:
    case 0xcb: case 0xcc: case 0xcd: case 0xe2: case 0xe7: case 0xe8:
    case 0xe9:
        return true;
    default:
        return type >= 0x6e && type <= 0x72;
    }
}

/* Complete C translation of Pharaoh.exe 0x00434820. */
bool pharaoh_minimap_storage_delivery_missing(
    PharaohMinimapNativeMetrics *context, int32_t building_id)
{
    PharaohBuildingTypeRecord *building = &context->buildings[building_id];
    bool set_missing = false;

    if (building->house_size != 0) return false;
    if (building->building_type == 0x5c || building->building_type == 0xb4) {
        set_missing = building->farm_water_access == 0;
    } else if (pharaoh_building_is_farm(context->buildings, building_id) ||
        (building->building_type >= 0x6a && building->building_type <= 0x6d)) {
        set_missing = delivery_walker_is_active(context, building->walker_id);
    } else if (type_uses_idle_delivery_rule(building->building_type)) {
        set_missing = delivery_walker_is_active(context, building->walker_id) ||
            building->loads_stored <= 0;
    }
    if (set_missing) building->native_c3 = 1;
    return building->native_c3 != 0;
}
