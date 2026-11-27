#include "pharaoh/map/construction_routes.h"
#include "pharaoh/core/memory_fill.h"
#include "pharaoh/map/ferry_routing.h"
#include <stddef.h>

_Static_assert(sizeof(void *)!=4 || offsetof(PharaohFloodTile,is_floodplain)==24,
    "native flood-object BYTE18");

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}
static uint32_t road_under(const PharaohConstructionRouteQueries *c,uint32_t tile)
{
    return (uint32_t)pharaoh_map_can_place_road_under_canal(c->terrain,c->image_ids,
        *c->group_f56b84,*c->orientation,signed_bits(tile));
}

/* Complete actual MenuRouteQueries C; retain raw masks and odd scratch slots. */
uint32_t pharaoh_map_adjacent_road_count(const PharaohConstructionRouteQueries *c,
    uint32_t tile)
{
    static const int32_t delta[4]={-228,1,228,-1};
    uint32_t flags[4];
    for(int32_t i=0;i<4;++i) flags[i]=c->terrain[tile+(uint32_t)delta[i]];
    for(int32_t i=0;i<4;++i) c->road_flags[i*2]=flags[i]&UINT32_C(0x08000040);
    for(int32_t i=0;i<4;++i){
        if(flags[i]&8u){
            const int32_t id=c->building_ids[tile+(uint32_t)delta[i]];
            const int16_t type=c->buildings[id].building_type;
            if(type==138 || type==136) c->road_flags[i*2]=1;
        }
    }
    return (c->road_flags[0]!=0)+(c->road_flags[2]!=0)+
        (c->road_flags[4]!=0)+(c->road_flags[6]!=0);
}

uint8_t pharaoh_map_road_has_crossing(const uint32_t *terrain,int32_t tile)
{
    return (uint8_t)pharaoh_map_diagonal_pair_has_mask(terrain,tile,
        UINT32_C(0x08000040));
}

uint32_t pharaoh_map_can_place_road_on_canal_object(
    const PharaohConstructionRouteQueries *c,uint32_t tile)
{
    if(c->terrain[tile]&256u) return road_under(c,tile);
    const PharaohFloodTile *const object=c->flood_tiles[tile];
    if(object==0 || object->is_floodplain!=0) return 1;
    const uint32_t image=(uint32_t)c->image_ids[tile]-(uint32_t)*c->group_f56c34;
    uint32_t side;
    if(image==52 || image==48) side=0;
    else if(image==54 || image==50) side=1;
    else return 0;
    if(*c->orientation==2 || *c->orientation==6) side^=1u;
    if(side){
        if(c->terrain[tile-228u]&64u) return 0;
        if(signed_bits(c->distances[tile-228u])>0) return 0;
        if(c->terrain[tile+228u]&64u) return 0;
        return signed_bits(c->distances[tile+228u])<=0;
    }
    if(c->terrain[tile+1u]&64u) return 0;
    if(signed_bits(c->distances[tile+1u])>0) return 0;
    if(c->terrain[tile-1u]&64u) return 0;
    return signed_bits(c->distances[tile-1u])<=0;
}

uint32_t pharaoh_map_can_place_canal_on_road(
    const PharaohConstructionRouteQueries *c,uint32_t tile)
{
    const int32_t id=c->building_ids[tile];
    if(id!=0 && c->buildings[id].building_type==138) return 1;
    const uint32_t flags=c->terrain[tile];
    uint32_t image;
    volatile int32_t base;
    if(flags&0x10000u){image=(uint32_t)c->image_ids[tile];base=*c->group_f56cd4;}
    else if(c->property_flags[tile]&128u){
        image=(uint32_t)c->image_ids[tile];base=*c->group_f5695c;
        if(signed_bits(image-(uint32_t)base)>=3) return 0;
        if(pharaoh_map_adjacent_road_count(c,tile)>2) return 0;
        return pharaoh_map_road_has_crossing(c->terrain,signed_bits(tile))==0;
    }else{
        const uint8_t buildable=pharaoh_map_tile_is_buildable_for_editor_at(
            c->terrain,c->height_values,signed_bits(tile));
        base=*c->group_f56c44; /* actual read precedes possible replacement */
        image=(uint32_t)c->image_ids[tile];
        if(buildable==0) base=*c->group_f56c94;
    }
    uint32_t side;
    switch(image-(uint32_t)base){
    case 0:case 2:case 8:case 10:side=1;break;
    case 1:case 3:case 9:case 11:side=0;break;
    default:return 0;
    }
    if(*c->orientation==2 || *c->orientation==6) side^=1u;
    if(side){
        if(c->terrain[tile-228u]&256u) return 0;
        return (c->terrain[tile+228u]&256u)==0;
    }
    if(c->terrain[tile+1u]&256u) return 0;
    return (c->terrain[tile-1u]&256u)==0;
}

static void enqueue(const PharaohConstructionRouteContext *c,uint32_t tile)
{
    uint32_t tail=*c->tail;
    c->queue[tail]=tile;
    ++tail;*c->tail=tail;
    if(signed_bits(tail)>51983) *c->tail=0;
}
static uint32_t advance_head(const PharaohConstructionRouteContext *c)
{
    uint32_t head=*c->head+1u;*c->head=head;
    if(signed_bits(head)>51983){head=0;*c->head=0;}
    return head;
}

/* Complete MenuRouteFlood reuse: N/E/S/W, actual sentinel/cost differences. */
uint32_t pharaoh_map_construction_distances(const PharaohConstructionRouteContext *c,
    int32_t x,int32_t y,int32_t mode)
{
    const PharaohConstructionRouteQueries *const q=c->queries;
    pharaoh_memory_fill_bytes(c->distances,0x32c40,0);
    uint32_t tile=(uint32_t)*c->map_origin+(uint32_t)y*228u+(uint32_t)x;
    *c->start=tile;
    uint32_t flags=q->terrain[tile];
    uint32_t rejected=(flags&UINT32_C(0x12040011))!=0;
    if(!rejected) rejected=c->routing[tile]==255;
    if(!rejected && (flags&256u)){
        rejected=road_under(q,tile)==0;tile=*c->start;
    }
    if(rejected){
        if(mode==0) return 0;
        if(!(flags&256u)){
            if(!(flags&8u)) return 0;
            const int32_t id=q->building_ids[tile];
            const int16_t type=q->buildings[id].building_type;
            if(type!=90 && type!=138) return 0;
        }
    }
    if(c->routing[tile]==2) return 0;
    if((flags&256u) && mode==0){
        if(road_under(q,tile)==0) return 0;
        tile=*c->start;
    }
    if(c->routing[tile]==251){
        if(mode!=0 || pharaoh_map_can_place_road_on_canal_object(q,tile)==0) return 0;
        tile=*c->start;
    }
    if((flags&64u) && mode!=0 && pharaoh_map_can_place_canal_on_road(q,tile)==0) return 0;
    *c->calls+=1u;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)c->marks,0xcb10,0);
    c->distances[*c->start]=1;c->queue[0]=*c->start;*c->head=0;*c->tail=1;
    static const uint32_t delta[4]={UINT32_C(0xffffff1c),1,228,UINT32_MAX};
    uint32_t head=0;
    do {
        const uint32_t origin=c->queue[head],distance=c->distances[origin]+1u;
        for(int32_t d=0;d<4;++d){
            const uint32_t next=origin+delta[d];
            if(c->distances[next]!=0) continue;
            flags=q->terrain[next];
            uint32_t blocked=0;
            if((flags&256u) && road_under(q,next)){
                if(mode==0 && road_under(q,next)==0){
                    c->distances[next]=UINT32_MAX;blocked=1;
                }
            }else{
                const uint8_t cost=c->routing[next];
                if(cost==251){
                    if(mode!=0) blocked=1;
                    else if(pharaoh_map_can_place_road_on_canal_object(q,next)==0){
                        c->distances[next]=UINT32_MAX;blocked=1;
                    }
                }else if(cost==2 || cost==255) blocked=1;
                else {
                    flags=q->terrain[next];
                    if(flags&UINT32_C(0x12040011)) blocked=1;
                    else if((flags&8u) && (cost!=(d==0?250:252) || mode!=1)) blocked=1;
                }
            }
            if((q->terrain[next]&64u) && mode!=0 && pharaoh_map_can_place_canal_on_road(q,next)==0){
                c->distances[next]=UINT32_MAX;continue;
            }
            if(blocked) continue;
            c->distances[next]=distance;
            if(c->routing[next]==254) c->distances[next]=distance+4u;
            enqueue(c,next);
        }
        head=advance_head(c);
    } while(head!=*c->tail);
    return 1;
}

uint32_t pharaoh_map_construction_wall_distances(const PharaohConstructionRouteContext *c,
    int32_t x,int32_t y)
{
    pharaoh_memory_fill_bytes(c->distances,0x32c40,0);
    *c->start=(uint32_t)*c->map_origin+(uint32_t)y*228u+(uint32_t)x;
    c->distances[*c->start]=1;c->queue[0]=*c->start;*c->head=0;*c->tail=1;
    static const uint32_t delta[4]={UINT32_C(0xffffff1c),1,228,UINT32_MAX};
    uint32_t head=0;
    do {
        const uint32_t tile=c->queue[head],distance=c->distances[tile]+1u;
        for(int32_t d=0;d<4;++d){
            const uint32_t next=tile+delta[d];
            if(c->distances[next]==0 && c->routing[next]==4){
                c->distances[next]=distance;enqueue(c,next);
            }
        }
        head=advance_head(c);
    } while(head!=*c->tail);
    return 1;
}

void pharaoh_construction_routes_bind_shared(PharaohConstructionRouteContext *c,
    PharaohConstructionRouteQueries *q,const struct PharaohFerryMapContext *map)
{
    q->terrain=map->terrain;
    q->orientation=(const int32_t *)(const void *)map->orientation;
    q->distances=map->distances;q->road_flags=map->adjacent_flags;
    c->queries=q;c->map_origin=(const int32_t *)(const void *)map->origin;
    c->routing=map->water_cost;c->distances=map->distances;c->queue=map->queue;
    c->start=map->active_tile;c->head=map->queue_head;c->tail=map->queue_tail;
}

void pharaoh_construction_routes_bind_groups(PharaohConstructionRouteQueries *q,
    const int32_t *groups)
{
    /* (native first-image address - F55BB4) / native group stride8. */
    q->group_f56b84=groups+506;q->group_f56c34=groups+528;
    q->group_f56cd4=groups+548;q->group_f5695c=groups+437;
    q->group_f56c44=groups+530;q->group_f56c94=groups+540;
}
