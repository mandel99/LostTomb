#include "pharaoh/map/land_route_fill.h"
#include "pharaoh/core/memory_fill.h"

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

uint32_t pharaoh_map_construction_checked_distances(
    const PharaohLandRouteFillContext *c,int32_t x,int32_t y,int32_t mode)
{
    const PharaohConstructionRouteContext *const r=c->route;
    const PharaohConstructionRouteQueries *const q=r->queries;
    pharaoh_memory_fill_bytes(r->distances,0x32c40,0);
    uint32_t tile=(uint32_t)*r->map_origin+(uint32_t)y*228u+(uint32_t)x;
    const uint8_t cost=r->routing[tile];
    *r->start=tile;
    const uint32_t flags=q->terrain[tile];
    if(cost==255 || (flags&UINT32_C(0x12040011))!=0){
        if(mode==0) return 0;
        if((flags&256u)==0){
            if((flags&8u)==0) return 0;
            const int16_t type=q->buildings[q->building_ids[tile]].building_type;
            if(type!=90 && type!=138) return 0;
        }
    }
    if(cost==2) return 0;
    if(flags&256u){
        if(road_under(q,tile)!=0 && mode==0 &&
            road_under(q,*r->start)==0) return 0;
        tile=*r->start;
    }
    if(r->routing[tile]==251){
        if(mode!=0 || pharaoh_map_can_place_road_on_canal_object(q,tile)==0) return 0;
        tile=*r->start;
    }
    if((flags&64u)!=0 && mode!=0 &&
        pharaoh_map_can_place_canal_on_road(q,tile)==0) return 0;
    if(pharaoh_map_grid_size_reaches_outside(c->geometry,
        signed_bits((uint32_t)*c->world_x+(uint32_t)x),
        signed_bits((uint32_t)*c->world_y+(uint32_t)y),3)) return 0;

    ++*r->calls;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)r->marks,0xcb10,0);
    r->distances[*r->start]=1;r->queue[0]=*r->start;
    *r->head=0;*r->tail=1;
    static const uint32_t delta[4]={UINT32_C(0xffffff1c),1,228,UINT32_MAX};
    uint32_t head=0;
    do {
        const uint32_t origin=r->queue[head],distance=r->distances[origin]+1u;
        for(int32_t d=0;d<4;++d){
            const uint32_t next=origin+delta[d];
            if(r->distances[next]!=0) continue;
            uint32_t blocked=0;
            if((q->terrain[next]&256u)!=0 && road_under(q,next)!=0){
                if(mode==0 && road_under(q,next)==0){
                    r->distances[next]=UINT32_MAX;blocked=1;
                }
            }else{
                const uint8_t next_cost=r->routing[next];
                if(next_cost==251){
                    if(mode!=0) blocked=1;
                    else if(pharaoh_map_can_place_road_on_canal_object(q,next)==0){
                        r->distances[next]=UINT32_MAX;blocked=1;
                    }
                }else if(next_cost==2 || next_cost==255) blocked=1;
                else {
                    const uint32_t next_flags=q->terrain[next];
                    if(next_flags&UINT32_C(0x12040011)) blocked=1;
                    else if(next_flags&8u){
                        /* Native north alone requires mode1 for type138.
                         * East/south/west admit that type in every mode. */
                        uint32_t admitted=0;
                        if(d!=0 || mode==1)
                            admitted=q->buildings[q->building_ids[next]].building_type==138;
                        if(!admitted && (next_cost!=(d==0 || d==2?250:252) || mode!=1))
                            blocked=1;
                    }
                }
            }
            if((q->terrain[next]&64u)!=0 && mode!=0 &&
                pharaoh_map_can_place_canal_on_road(q,next)==0){
                r->distances[next]=UINT32_MAX;continue;
            }
            if(blocked) continue;
            const int32_t index=signed_bits(next);
            if(pharaoh_map_grid_size_reaches_outside(c->geometry,index%228,index/228,3))
                continue;
            r->distances[next]=distance;
            if(r->routing[next]==254) r->distances[next]=distance+4u;
            uint32_t tail=*r->tail;
            r->queue[tail]=next;*r->tail=++tail;
            if(signed_bits(tail)>51983) *r->tail=0;
        }
        head=*r->head+1u;*r->head=head;
        if(signed_bits(head)>51983){head=0;*r->head=0;}
    }while(head!=*r->tail);
    return 1;
}
