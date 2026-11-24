#ifndef PHARAOH_RENDERING_MINIMAP_NATIVE_METRICS_H
#define PHARAOH_RENDERING_MINIMAP_NATIVE_METRICS_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/walker/record_queries.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct PharaohMinimapNativeMetrics {
    PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    const uint8_t *map_edge;
    const int32_t *map_image;
    int32_t *active_sprite_image;
    const int32_t *current_map_cell;
    const int32_t *view_orientation;
    const int32_t *active_tax_building_counts; /* five native counters */
} PharaohMinimapNativeMetrics;

int32_t pharaoh_minimap_fire_risk_height(
    const PharaohMinimapNativeMetrics *context, int32_t building_id);
int32_t pharaoh_minimap_damage_risk_height(
    const PharaohMinimapNativeMetrics *context, int32_t building_id);
int32_t pharaoh_minimap_tax_income_height(
    const PharaohMinimapNativeMetrics *context, int32_t building_id);
bool pharaoh_minimap_storage_delivery_missing(
    PharaohMinimapNativeMetrics *context, int32_t building_id);

#endif
