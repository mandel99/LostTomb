#ifndef PHARAOH_RENDERING_OVERLAY_METRICS_H
#define PHARAOH_RENDERING_OVERLAY_METRICS_H

#include "pharaoh/rendering/overlay_columns.h"

#include <stdbool.h>
#include <stdint.h>

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

#endif
