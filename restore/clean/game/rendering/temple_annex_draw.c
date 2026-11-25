#include "pharaoh/rendering/temple_annex_draw.h"
#include "pharaoh/rendering/upper_sprite_draw.h"

static int32_t add_i32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}

static void draw_top(PharaohTempleAnnexDrawContext *c,int32_t image,
    int32_t x,int32_t y)
{
    const PharaohUpperSpriteDrawContext top={
        c->draw->images,c->draw->image_pixels,*c->draw->image_count,
        c->draw->clip,c->draw->surface_pixels,c->draw->pitch_bytes,
        *c->draw->tint,&c->draw->state->source.pixels};
    (void)pharaoh_image_draw_isometric_top(&top,image,x,y);
}

/* Complete typed C translation of Pharaoh.exe 0x004C0160. */
void pharaoh_temple_complex_draw_annex(PharaohTempleAnnexDrawContext *c,
    int32_t building_id)
{
    const PharaohBuildingTypeRecord *const b=&c->buildings[building_id];
    int32_t god=c->god_with_status(c->user_data,2);
    if(god<0) {
        god=c->god_with_status(c->user_data,1);
        if(god<0) god=0;
    }
    const int32_t god_group=c->god_image_groups[god];
    const int32_t direction=b->reserved_aa[0];
    int32_t facing=add_i32(direction,c->map_orientation);
    if(facing>7) facing-=8;
    int32_t x_adjust=64,y_adjust=-90,flip=0;
    switch(facing) {
    case 0:case 3:case 4:case 7:
        x_adjust=-1;y_adjust=-65;flip=1;break;
    case 1:case 2:case 5:case 6:
        x_adjust=12;y_adjust=-64;break;
    default:break;
    }
    for(int32_t slot=2;slot<=3;++slot) {
        const PharaohTempleAnnexPart *const part=
            &c->parts[c->orientation_parts[facing*4+slot]];
        int32_t tile_x=b->x,tile_y=b->y;
        if(direction==0) {
            tile_x=add_i32(tile_x,part->dx);
            tile_y=add_i32(tile_y,part->dy);
        } else if(direction==1) {
            tile_x=add_i32(tile_x,part->dy);
            tile_y=add_i32(tile_y,part->dx);
        }
        int32_t image,shift_x=0,shift_y=0;
        if(slot==2) {
            image=add_i32(c->statue_image_first,flip);
            shift_x=x_adjust;shift_y=y_adjust;
        } else {
            const int32_t delta=part->size-1;
            if(c->map_orientation==2) tile_x=add_i32(tile_x,delta);
            else if(c->map_orientation==4) {
                tile_x=add_i32(tile_x,delta);tile_y=add_i32(tile_y,delta);
            } else if(c->map_orientation!=0) tile_y=add_i32(tile_y,delta);
            image=add_i32(c->image_group_first[god_group],flip);
        }
        int32_t pixel_x=0,pixel_y=0;
        c->project_tile(c->user_data,tile_x,tile_y,&pixel_x,&pixel_y);
        pixel_x=add_i32(pixel_x,shift_x);pixel_y=add_i32(pixel_y,shift_y);
        if(b->state==6)*c->tint=UINT16_C(0xf863);
        (void)pharaoh_image_draw(c->draw,(uint32_t)image,
            (uint32_t)pixel_x,(uint32_t)pixel_y);
        draw_top(c,image,pixel_x,pixel_y);
        *c->tint=0;
    }
}
