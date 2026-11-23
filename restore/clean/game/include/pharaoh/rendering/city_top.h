#ifndef PHARAOH_RENDERING_CITY_TOP_H
#define PHARAOH_RENDERING_CITY_TOP_H
#include "pharaoh/rendering/city_ground.h"
#include "pharaoh/rendering/building_animations.h"

typedef struct PharaohCityTopOperations {
 void *user_data;
 void (*draw_pyramid)(void *,int32_t a0,int32_t a1,int32_t a2,int32_t a3); /*004EF550*/
 void (*draw_abu_simbel)(void *,int32_t a0,int32_t a1,int32_t a2); /*00401F70*/
 void (*draw_00420a30)(void *,int32_t a0,int32_t a1); /*00420A30*/
 uint8_t (*is_farm)(void *,int32_t a0); /*00488030*/
 void (*draw_farm)(void *,int32_t a0,int32_t a1,int32_t a2,int32_t a3); /*00488C20*/
 void (*draw_0041fff0)(void *,int32_t a0,int32_t a1); /*0041FFF0*/
 void (*draw_004e5e90)(void *,int32_t a0,int32_t a1,int32_t a2); /*004E5E90*/
 void (*draw_00542e10)(void *,int32_t a0,int32_t a1,int32_t a2); /*00542E10*/
 void (*draw_00420050)(void *,int32_t a0,int32_t a1); /*00420050*/
 void (*draw_004206f0)(void *,int32_t a0,int32_t a1); /*004206F0*/
 void (*draw_00420d60)(void *,int32_t a0,int32_t a1); /*00420D60*/
 void (*draw_00421920)(void *,int32_t a0,int32_t a1,int32_t a2); /*00421920*/
 void (*draw_00421060)(void *,int32_t a0,int32_t a1); /*00421060*/
 void (*draw_walker)(void *,int32_t a0,int32_t a1,int32_t a2,uint32_t selection); /*00438E50*/
 void (*draw_granary_stores)(void *,int32_t a0,int32_t a1,int32_t a2,int32_t a3); /*00442280*/
 int32_t (*animation_offset)(void *,int32_t a0,int32_t a1); /*0040F2C0*/
 uint8_t (*is_floodplain_farm)(void *,int32_t a0); /*00488050*/
 void (*draw_004c0160)(void *,int32_t a0,int32_t a1,int32_t a2); /*004C0160*/
 void (*draw_building_details)(void *,int32_t a0,int32_t a1,int32_t a2); /*00436310*/
} PharaohCityTopOperations;
typedef struct PharaohCityTopContext {
 PharaohCityGroundContext *ground;
 PharaohImageDrawContext *draw;
 const PharaohBuildingAnimationContext *animations;
 const uint8_t *sprites;
 const uint32_t *orientation;
 uint16_t *pass_sequence;
 const PharaohCityTopOperations *operations;
} PharaohCityTopContext;
void pharaoh_city_draw_top(PharaohCityTopContext *,uint32_t actor_parameter);
void pharaoh_city_render_bind_top(PharaohCityRenderOperations *,PharaohCityTopContext *);
void pharaoh_city_top_draw_type_animation(PharaohCityTopContext *,int32_t image,uint32_t frame,int32_t building,uint32_t footprint,int32_t x,int32_t y);
#endif
