#include "pharaoh/rendering/overlay_metrics.h"

#include "pharaoh/core/percentage_scale.h"

int32_t pharaoh_overlay_fertility_height(
    const PharaohOverlayMetricContext *c, int32_t offset)
{
    uint8_t value = c->soil_fertility[offset];
    if (value == 0u) return 0;
    if (value < 10u) return 1;
    if (value < 20u) return 2;
    if (value < 30u) return 3;
    if (value < 40u) return 4;
    if (value < 50u) return 5;
    if (value < 60u) return 6;
    if (value < 70u) return 7;
    if (value < 80u) return 8;
    return 9;
}

int32_t pharaoh_overlay_desirability_height(
    const PharaohOverlayMetricContext *c, int32_t offset)
{
    int8_t value = c->desirability[offset];
    if ((c->terrain[offset] & c->terrain_building_flag) == 0u && value == 0)
        return 0;
    if (value < -10) return 0;
    if (value < -5) return 1;
    if (value < 0) return 2;
    if (value == 0) return 3;
    if (value < 5) return 4;
    if (value < 10) return 5;
    if (value < 15) return 6;
    if (value < 20) return 7;
    return value > 24 ? 9 : 8;
}

int32_t pharaoh_overlay_malaria_height(const PharaohOverlayBuilding *b)
{ return b->house_size == 0u ? 0 : b->malaria_risk / 100; }

int32_t pharaoh_overlay_disease_height(const PharaohOverlayBuilding *b)
{ return b->house_size == 0u ? 0 : b->plague_risk / 100; }

int32_t pharaoh_overlay_crime_height(const PharaohOverlayMetricContext *c,
    const PharaohOverlayBuilding *b)
{
    if (b->house_size == 0u || c->crime_height_excluded(c->user_data, b)) return 0;
    return b->crime_risk / 100;
}

int32_t pharaoh_overlay_labor_height(const PharaohOverlayBuilding *b)
{
    if (b->house_size != 0u) return 0;
    /* Native 0x00434A2B sign-extends the 16-bit building field at +0x18. */
    return pharaoh_scale_value_to_percent((int16_t)b->houses_covered,
        300).value / 10;
}

int32_t pharaoh_overlay_religion_height(const PharaohOverlayMetricContext *c,
    const PharaohOverlayBuilding *b)
{
    int32_t god, kind, count = 0;
    if (b->house_size == 0u || c->religion_height_excluded(c->user_data, b)) return 0;
    for (god = 0; god < c->god_count; ++god) for (kind = 0; kind < 3; ++kind)
        count += c->active_building_count(c->user_data,
            c->building_type_for_god(c->user_data, god, kind));
    if (c->oracle_total + count == 0) return 0;
    return (b->num_gods * 17) / 10;
}

int32_t pharaoh_overlay_food_stock_height(
    const PharaohOverlayFoodBuilding *b,
    const int32_t *model_food_type_counts, int32_t food_supply_available)
{
    int32_t population = 0;
    int32_t stored = 0;
    int32_t percentage;
    int32_t index;

    if (b->type == 0x46 || b->type == 0xbe || b->type == 0x4c ||
        b->type == 0x47 || b->type == 0xbf || b->house_size == 0u ||
        food_supply_available == 0) {
        return 0;
    }
    if (model_food_type_counts[b->house_level] != 0) {
        population = b->population;
    }
    for (index = 0; index < 8; ++index) {
        stored += b->stored_food[index];
    }
    if (population == 0) {
        return 0;
    }
    percentage = pharaoh_scale_value_to_percent(stored, population).value;
    if (percentage > 200) {
        return 0;
    }
    if (percentage >= 100) {
        return 1;
    }
    return percentage <= 0 ? 10 : 5;
}

int32_t pharaoh_overlay_damage_terrain_height(
    const int16_t *damage_terrain, int32_t grid_offset)
{
    return damage_terrain[grid_offset] / 100;
}

int32_t pharaoh_building_type_overlay_tile_index(
    const int32_t *overlay_image_ids, int32_t building_type)
{
    if (building_type >= 0 && building_type < 0xed &&
        overlay_image_ids[building_type] > 0) {
        return overlay_image_ids[building_type] - 1;
    }
    return -1;
}

int32_t pharaoh_overlay_farm_progress_percent(
    int16_t progress, int32_t maximum_progress)
{
    return pharaoh_scale_value_to_percent(
        progress, maximum_progress).value;
}
