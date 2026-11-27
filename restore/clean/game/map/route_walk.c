#include "pharaoh/map/route_walk.h"

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}
static uint32_t road_under(const PharaohConstructionRouteQueries *q,uint32_t tile)
{
    return (uint32_t)pharaoh_map_can_place_road_under_canal(q->terrain,q->image_ids,
        *q->group_f56b84,*q->orientation,signed_bits(tile));
}

uint32_t pharaoh_map_place_routed_building(const PharaohRouteWalkContext *c,
    int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t mode)
{
    const PharaohConstructionRouteQueries *const q=c->queries;
    *c->tiles->placed_count=0;
    uint32_t tile=(uint32_t)*c->map_origin+(uint32_t)y*228u+(uint32_t)x,count=1;
    *c->current_tile=tile;
    for(;;){
        const int32_t distance=signed_bits(c->distances[tile]);
        if(distance<=0 && q->buildings[q->building_ids[tile]].building_type!=138)
            return 0;
        switch(mode){
        case 0:
            if((q->terrain[tile]&256u)!=0 && road_under(q,tile)==0) return 0;
            pharaoh_map_set_road_tile(c->tiles,x,y,1);break;
        case 2: ++*c->tiles->placed_count;break;
        case 3:
            if((q->terrain[tile]&64u)!=0 &&
                pharaoh_map_can_place_canal_on_road(q,tile)==0) return 0;
            pharaoh_map_set_canal_tile(c->tiles,x,y,1,1);break;
        default:break;
        }
        const uint8_t direction=(uint8_t)pharaoh_map_general_direction(x,y,target_x,target_y);
        if(direction==8) return 1;
        int32_t i,step=0;
        for(i=0;i<4;++i){
            step=c->preferred_directions[direction][i];
            tile=*c->current_tile+(uint32_t)c->tile_deltas[step];
            const int32_t next=signed_bits(c->distances[tile]);
            if(next>0 && next<distance &&
                ((q->terrain[tile]&256u)==0 || road_under(q,tile)!=0)) break;
        }
        if(i==4) return 0;
        x=signed_bits((uint32_t)x+(uint32_t)c->coordinate_deltas[step].x);
        y=signed_bits((uint32_t)y+(uint32_t)c->coordinate_deltas[step].y);
        *c->current_tile=tile;
        if(++count>=400u) return 0;
    }
}
