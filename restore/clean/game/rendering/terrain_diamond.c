#include "pharaoh/rendering/terrain_diamond.h"
#include <string.h>

/* Reused TerrainDiamond C: packed rows2,6,...,58,58,...,6,2. Edge paths
 * crop30 pixels from the left or retain28 at the right. */
static void diamond(const PharaohTerrainRasterTarget *target,
    const uint8_t *source,int32_t vertical_clip,int32_t side,int32_t masked)
{
    const uint32_t offset=(uint32_t)target->x*2u+
        (uint32_t)target->y*(uint32_t)target->pitch_bytes;
    uint8_t *const destination=target->surface_pixels+(int32_t)offset;
    uint32_t output_row=0;
    for(uint32_t row=0;row<30;++row) {
        const uint32_t half=row<15?row:29u-row;
        const uint32_t width=half*4u+2u,left=(58u-width)/2u;
        uint32_t start=left,end=left+width;
        if(vertical_clip==3 && row<15) {source+=width*2u;continue;}
        if(vertical_clip==4 && row>=15)break;
        if(side==1 && start<30)start=30;
        if(side==2 && end>28)end=28;
        for(uint32_t column=start;column<end;++column) {
            uint16_t value;
            const uint32_t x=column-(side==1?30u:0u);
            uint32_t source_column=column-left;
            /* Possible original raster bugs: the left edge repeats one DWORD
             * on row15 and retains an earlier high EAX WORD on row25.
             * Preserve the native visible pixels in both masked variants. */
            if(side==1 && ((row==15 && (column==38 || column==39)) ||
                (row==25 && column==37)))source_column-=2;
            memcpy(&value,source+source_column*2u,2);
            if(masked)value&=target->tint;
            memcpy(destination+output_row*(uint32_t)target->pitch_bytes+x*2u,&value,2);
        }
        source+=width*2u;
        ++output_row;
    }
}

void pharaoh_terrain_diamond_mask(const PharaohTerrainRasterTarget *target,
    const uint8_t *pixels,int32_t clip) {diamond(target,pixels,clip,0,1);}
void pharaoh_terrain_diamond_draw(const PharaohTerrainRasterTarget *target,
    const uint8_t *pixels,int32_t clip)
{
    if(target->tint!=0)pharaoh_terrain_diamond_mask(target,pixels,clip);
    else diamond(target,pixels,clip,0,0);
}
void pharaoh_terrain_diamond_mask_left(const PharaohTerrainRasterTarget *target,
    const uint8_t *pixels,int32_t clip) {diamond(target,pixels,clip,1,1);}
void pharaoh_terrain_diamond_draw_left(const PharaohTerrainRasterTarget *target,
    const uint8_t *pixels,int32_t clip)
{
    if(target->tint!=0)pharaoh_terrain_diamond_mask_left(target,pixels,clip);
    else diamond(target,pixels,clip,1,0);
}
void pharaoh_terrain_diamond_mask_right(const PharaohTerrainRasterTarget *target,
    const uint8_t *pixels,int32_t clip) {diamond(target,pixels,clip,2,1);}
void pharaoh_terrain_diamond_draw_right(const PharaohTerrainRasterTarget *target,
    const uint8_t *pixels,int32_t clip)
{
    if(target->tint!=0)pharaoh_terrain_diamond_mask_right(target,pixels,clip);
    else diamond(target,pixels,clip,2,0);
}

static int32_t footprint(PharaohTerrainFootprintContext *context,uint32_t size)
{
    PharaohSpriteClipState *const clip=context->clip;
    const uint32_t x=(uint32_t)clip->draw_x,y=(uint32_t)clip->draw_y;
    uint32_t tile=0;
    clip->image_width=58;
    clip->image_height=30;
    for(uint32_t diagonal=0;diagonal<2u*size-1u;++diagonal) {
        const uint32_t count=diagonal<size?diagonal+1u:2u*size-1u-diagonal;
        const uint32_t left=x-(count-1u)*30u;
        for(uint32_t column=0;column<count;++column,++tile) {
            clip->draw_x=(int32_t)(left+column*60u);
            clip->draw_y=(int32_t)(y+diagonal*15u);
            pharaoh_graphics_clip_sprite_horizontal(clip);
            pharaoh_graphics_clip_sprite_vertical(clip);
            if(clip->vertical_clip==5)continue;
            const uint8_t *const pixels=context->pixels+tile*1800u;
            const PharaohTerrainRasterTarget target={context->surface_pixels,
                context->pitch_bytes,clip->draw_x,clip->draw_y,context->tint};
            if(clip->horizontal_clip==1)
                pharaoh_terrain_diamond_draw_left(&target,pixels,clip->vertical_clip);
            else if(clip->horizontal_clip==2)
                pharaoh_terrain_diamond_draw_right(&target,pixels,clip->vertical_clip);
            else pharaoh_terrain_diamond_draw(&target,pixels,clip->vertical_clip);
        }
    }
    return 58;
}

int32_t pharaoh_terrain_footprint_draw2(PharaohTerrainFootprintContext *c) {return footprint(c,2);}
int32_t pharaoh_terrain_footprint_draw3(PharaohTerrainFootprintContext *c) {return footprint(c,3);}
int32_t pharaoh_terrain_footprint_draw4(PharaohTerrainFootprintContext *c) {return footprint(c,4);}
int32_t pharaoh_terrain_footprint_draw5(PharaohTerrainFootprintContext *c) {return footprint(c,5);}
int32_t pharaoh_terrain_footprint_draw6(PharaohTerrainFootprintContext *c) {return footprint(c,6);}
