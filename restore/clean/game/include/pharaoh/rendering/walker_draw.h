#ifndef PHARAOH_RENDERING_WALKER_DRAW_H
#define PHARAOH_RENDERING_WALKER_DRAW_H

#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/rendering/walker_overlay.h"

typedef struct PharaohFormationDrawRecord {
    uint8_t reserved_00[2], legion_id, reserved_03[0x76];
    int16_t experience;
    uint8_t reserved_7c[6];
    int16_t request_event_id;
    int32_t ship_walker_id;
    uint8_t reserved_88[8];
} PharaohFormationDrawRecord;

typedef struct PharaohWalkerDrawOperations {
    void *user_data;
    void (*pyramid_walker_offset)(void *,int32_t,int32_t *,int32_t *); /*004F0D30*/
    void (*draw_number)(void *,int32_t,int32_t,const char *,int32_t,int32_t,int32_t,int32_t); /*004CCE00*/
} PharaohWalkerDrawOperations;

typedef struct PharaohWalkerDrawContext {
    const PharaohWalkerQueryRecord *walkers;
    const PharaohFormationDrawRecord *formations;
    const PharaohSpriteGroupRecord *groups;
    PharaohImageDrawContext *draw;
    const PharaohWalkerOverlayContext *overlay;
    const uint32_t *orientation;
    const int32_t *tile_half_width,*tile_half_height;
    int32_t *selected_x,*selected_y;
    const uint8_t *skip_draw;
    const PharaohWalkerDrawOperations *operations;
} PharaohWalkerDrawContext;

void pharaoh_city_draw_walker(PharaohWalkerDrawContext *,int32_t walker_id,
    int32_t x,int32_t y,uint32_t selection);

#endif
