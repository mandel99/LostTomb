#ifndef PHARAOH_RENDERING_BUILDING_ANIMATIONS_H
#define PHARAOH_RENDERING_BUILDING_ANIMATIONS_H
#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/walker/record_queries.h"

typedef struct PharaohBuildingAnimationContext {
 PharaohImageDrawContext *draw;
 const PharaohSpriteGroupRecord *groups;
 const PharaohWalkerQueryRecord *walkers;
} PharaohBuildingAnimationContext;

int32_t pharaoh_building_draw_sprite_at_offset(const PharaohBuildingAnimationContext *c,int32_t image,int32_t x,int32_t y,int32_t dx,int32_t dy);
void pharaoh_building_draw_shipwright_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y,uint32_t holder);
int32_t pharaoh_building_draw_fishing_wharf_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y,int32_t boat);
int32_t pharaoh_building_draw_mine_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y);
int32_t pharaoh_building_draw_quarry_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y);
int32_t pharaoh_building_draw_bandstand_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y);
int32_t pharaoh_building_draw_conservatory_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y);
int32_t pharaoh_building_draw_booth_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y);
int32_t pharaoh_building_draw_pavilion_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y);
int32_t pharaoh_building_draw_dance_school_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y);
#endif
