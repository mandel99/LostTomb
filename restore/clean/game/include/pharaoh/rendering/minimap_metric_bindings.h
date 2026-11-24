#ifndef PHARAOH_RENDERING_MINIMAP_METRIC_BINDINGS_H
#define PHARAOH_RENDERING_MINIMAP_METRIC_BINDINGS_H

#include "pharaoh/rendering/minimap_building.h"
#include "pharaoh/rendering/minimap_native_metrics.h"
#include "pharaoh/rendering/overlay_metrics.h"

typedef struct PharaohMinimapMetricBindings {
    PharaohMinimapNativeMetrics *native;
    const PharaohOverlayMetricContext *tile_metrics;
    const int16_t *damage_terrain;
    const int32_t *house_model_food_type_counts;
    const int32_t *food_supply_available;
    const int32_t *oracle_total;
    const int32_t *temple_counts; /* 15 native active-building counters */
    const int32_t *venue_counts;  /* nine native active-building counters */
    const int32_t *overlay_type_list;
    /* Required live native counters for service modes. */
    const int32_t *service_enabled[51][3];
} PharaohMinimapMetricBindings;

void pharaoh_minimap_bind_native_metrics(
    PharaohMinimapBuilding *building, PharaohMinimapMetricBindings *bindings);

#endif
