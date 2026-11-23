#ifndef PHARAOH_RENDERING_OVERLAY_TOP_H
#define PHARAOH_RENDERING_OVERLAY_TOP_H
#include "pharaoh/rendering/city_top.h"
typedef struct PharaohOverlayTopOperations {
 void *user_data;
 uint8_t (*shows_building)(void *,int32_t building); /*431690 AL*/
 void (*draw_column)(void *,uint32_t overlay,int32_t building,int32_t x,int32_t y);
 void (*draw_water_top)(void *,int32_t x,int32_t y); /*432320*/
 void (*draw_tomb_top)(void *,int32_t x,int32_t y); /*432820*/
 void (*draw_desirability_top)(void *,int32_t x,int32_t y); /*4339C0*/
 void (*draw_fertility_top)(void *,int32_t x,int32_t y); /*435050*/
 void (*draw_resource)(void *,int32_t x,int32_t y,int32_t resource,int32_t slot); /*434AA0*/
} PharaohOverlayTopOperations;
typedef struct PharaohOverlayTopContext {
 PharaohCityTopContext *top;
 const PharaohOverlayTopOperations *operations;
} PharaohOverlayTopContext;
void pharaoh_city_draw_overlay_top(PharaohOverlayTopContext *);
void pharaoh_city_render_bind_overlay_top(PharaohCityRenderOperations *,PharaohOverlayTopContext *);
#endif
