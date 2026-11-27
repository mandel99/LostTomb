#ifndef PHARAOH_RENDERING_OVERLAY_METRICS_H
#define PHARAOH_RENDERING_OVERLAY_METRICS_H

#include "pharaoh/rendering/overlay_columns.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct PharaohOverlayFoodBuilding {
    int16_t type;
    uint8_t house_size;
    int16_t house_level;
    int16_t population;
    int16_t stored_food[8];
} PharaohOverlayFoodBuilding;

typedef bool (*PharaohOverlayBuildingPredicate)(
    void *user_data, const PharaohOverlayBuilding *building);
typedef int32_t (*PharaohGodBuildingTypeFunction)(
    void *user_data, int32_t god, int32_t kind);
typedef int32_t (*PharaohBuildingCountFunction)(
    void *user_data, int32_t building_type);

typedef struct PharaohOverlayMetricContext {
    const uint8_t *soil_fertility;
    const int8_t *desirability;
    const uint32_t *terrain;
    uint32_t terrain_building_flag;
    int32_t god_count;
    int32_t oracle_total;
    PharaohOverlayBuildingPredicate crime_height_excluded;
    PharaohOverlayBuildingPredicate religion_height_excluded;
    PharaohGodBuildingTypeFunction building_type_for_god;
    PharaohBuildingCountFunction active_building_count;
    void *user_data;
} PharaohOverlayMetricContext;

int32_t pharaoh_overlay_fertility_height(
    const PharaohOverlayMetricContext *context, int32_t grid_offset);
int32_t pharaoh_overlay_desirability_height(
    const PharaohOverlayMetricContext *context, int32_t grid_offset);
int32_t pharaoh_overlay_malaria_height(
    const PharaohOverlayBuilding *building);
int32_t pharaoh_overlay_disease_height(
    const PharaohOverlayBuilding *building);
int32_t pharaoh_overlay_crime_height(const PharaohOverlayMetricContext *context,
    const PharaohOverlayBuilding *building);
int32_t pharaoh_overlay_labor_height(const PharaohOverlayBuilding *building);
int32_t pharaoh_overlay_religion_height(
    const PharaohOverlayMetricContext *context,
    const PharaohOverlayBuilding *building);

int32_t pharaoh_overlay_food_stock_height(
    const PharaohOverlayFoodBuilding *building,
    const int32_t *model_food_type_counts, int32_t food_supply_available);
int32_t pharaoh_overlay_damage_terrain_height(
    const int16_t *damage_terrain, int32_t grid_offset);
int32_t pharaoh_building_type_overlay_tile_index(
    const int32_t *overlay_image_ids, int32_t building_type);
#endif
