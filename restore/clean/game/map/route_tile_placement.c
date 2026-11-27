#include "pharaoh/map/route_tile_placement.h"

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

static void set_tile(const PharaohRouteTilePlacementContext *c,
    int32_t x,int32_t y,int32_t alternate,int32_t set,uint32_t mask)
{
    PharaohMapSurfaceContext *const s=c->surface;
    if(pharaoh_map_grid_is_inside(s->geometry,
        signed_bits((uint32_t)*c->world_x+(uint32_t)x),
        signed_bits((uint32_t)*c->world_y+(uint32_t)y))==0) return;
    const uint32_t origin=(uint32_t)s->map->origin;
    const uint32_t tile=origin+(uint32_t)y*228u+(uint32_t)x;
    const uint32_t flags=s->map->terrain[tile];
    if((flags&mask)==0) ++*c->placed_count;
    s->map->terrain[tile]=set==1?flags|mask:flags&~mask;
    s->property_flags[tile]&=0xefu;

    uint32_t left=(uint32_t)x-1u,top=(uint32_t)y-1u;
    uint32_t width=3,height=3;
    if(signed_bits(left)<0){width=left+3u;left=0;}
    else if(signed_bits(left+3u)>s->map->width)
        width=(uint32_t)s->map->width-left;
    if(signed_bits(top)<0){height=top+3u;top=0;}
    else if(signed_bits(top+3u)>s->map->height)
        height=(uint32_t)s->map->height-top;
    const uint32_t end_x=left+width,end_y=top+height;
    *s->scan_y=signed_bits(top);
    *s->current_tile=origin+top*228u+left;
    if(signed_bits(top)<signed_bits(end_y)) do {
        *s->scan_x=signed_bits(left);
        if(signed_bits(left)<signed_bits(end_x)) do {
            const uint32_t current=*s->current_tile;
            const uint32_t current_flags=s->map->terrain[current];
            if((current_flags&mask)!=0 && (current_flags&12u)==0){
                if(mask==64u){
                    pharaoh_map_adjacent_terrain_any(s->adjacent,
                        s->map->terrain,current,UINT32_C(0x08000040));
                    pharaoh_map_adjacent_add_gatehouse_flags(s);
                    pharaoh_map_set_road_image(s);
                }else if((s->property_flags[current]&128u)==0){
                    pharaoh_map_adjacent_channel_flags(s);
                    pharaoh_map_set_channel_image(s,alternate);
                }
            }
            *s->scan_x=signed_bits((uint32_t)*s->scan_x+1u);
            *s->current_tile+=1u;
        }while(*s->scan_x<signed_bits(end_x));
        *s->scan_y=signed_bits((uint32_t)*s->scan_y+1u);
        *s->current_tile+=228u-width;
    }while(*s->scan_y<signed_bits(end_y));
    pharaoh_map_refresh_surrounding_empty(s,signed_bits(left),
        signed_bits(top),signed_bits(width));
}

void pharaoh_map_set_road_tile(const PharaohRouteTilePlacementContext *c,
    int32_t x,int32_t y,int32_t set)
{
    set_tile(c,x,y,0,set,64u);
}

void pharaoh_map_set_canal_tile(const PharaohRouteTilePlacementContext *c,
    int32_t x,int32_t y,int32_t alternate,int32_t set)
{
    set_tile(c,x,y,alternate,set,256u);
}
