#ifndef PHARAOH_RENDERING_CITY_OBJECT_LAYERS_H
#define PHARAOH_RENDERING_CITY_OBJECT_LAYERS_H
#include "pharaoh/rendering/city_ground.h"

typedef struct PharaohCityObjectLayerOperations {
 void *user_data;
 void (*draw_walker)(void *,int32_t actor,int32_t x,int32_t y,uint32_t selection);
 void (*draw_grid_value)(void *,int32_t x,int32_t y);
} PharaohCityObjectLayerOperations;
typedef struct PharaohCityObjectLayerContext {
 PharaohCityGroundContext *ground;
 const uint8_t *cycle_enabled;
 uint32_t *cycle_counter;
 const uint32_t *debug_grid_a, *debug_grid_b, *debug_grid_c;
 const PharaohCityObjectLayerOperations *operations;
} PharaohCityObjectLayerContext;
void pharaoh_city_draw_high_walkers(PharaohCityObjectLayerContext *context,uint32_t selection);
void pharaoh_city_draw_grid_values(PharaohCityObjectLayerContext *context);
void pharaoh_city_render_bind_object_layers(PharaohCityRenderOperations *operations,
 PharaohCityObjectLayerContext *context);
#endif
