#include "pharaoh/map/ferry_routing.h"
#include "pharaoh/scenario/scene_routing.h"
#include "pharaoh/map/tile_adjacency.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/core/memory_fill.h"

static int32_t signed_bits(uint32_t value)
{
    return value<=INT32_MAX?(int32_t)value:
        (int32_t)((int64_t)value-INT64_C(0x100000000));
}

uint32_t pharaoh_map_shore_pattern(const PharaohFerryMapContext *c,int32_t x,int32_t y)
{
    const uint32_t orientation=*c->orientation;
    if(orientation==6) y=signed_bits((uint32_t)y-1);
    else if(orientation==4){x=signed_bits((uint32_t)x-1);y=signed_bits((uint32_t)y-1);}
    else if(orientation==2) x=signed_bits((uint32_t)x-1);
    const uint32_t tile=*c->origin+(uint32_t)y*228+(uint32_t)x;
    for(uint32_t pattern=0;pattern<4;++pattern){
        uint32_t matches=0;
        for(uint32_t side=0;side<4;++side){
            const uint32_t wanted=c->shore_patterns[pattern][side];
            const uint32_t at=tile+(uint32_t)c->corner_offsets[side];
            if((wanted!=0)==((c->terrain[at]&4)!=0)) ++matches;
        }
        if(matches==4) return pattern;
    }
    return 999;
}

uint32_t pharaoh_map_water_tile_next_to(PharaohFerryMapContext *c,int32_t x,int32_t y,uint32_t size)
{
    const uint32_t tile=*c->origin+(uint32_t)y*228+(uint32_t)x;
    for(uint32_t i=0;i<24;++i){
        const int32_t delta=c->ring_offsets[size*24+i];
        if(delta==0) return 0;
        const uint32_t at=tile+(uint32_t)delta;
        const uint32_t flags=c->terrain[at];
        if((flags&4)!=0 && (flags&8)==0){
            *c->current_tile=at;
            pharaoh_map_adjacent_terrain_any(c->adjacent_flags,c->terrain,
                signed_bits(at),4);
            if(pharaoh_map_adjacent_flags_count(c->adjacent_flags)>=8){
                *c->result_x=signed_bits((uint32_t)c->ring_xy[size*24+i][0]+(uint32_t)x);
                *c->result_y=signed_bits((uint32_t)c->ring_xy[size*24+i][1]+(uint32_t)y);
                return 1;
            }
        }
    }
    return 0;
}

uint32_t pharaoh_map_ferry_water_distances(PharaohFerryMapContext *c,int32_t x,int32_t y,int32_t end_x,int32_t end_y)
{
    static const int32_t directions[4]={-228,1,228,-1};
    static const int32_t nearby[8]={-228,-227,1,229,228,227,-1,-229};
    pharaoh_memory_fill_bytes(c->distances,0x32c40,0);
    const uint32_t start=*c->origin+(uint32_t)y*228+(uint32_t)x;
    *c->active_tile=start;
    const uint32_t end=*c->origin+(uint32_t)end_y*228+(uint32_t)end_x;
    if(c->water_cost[start]==255) return 0;
    c->distances[start]=1;c->queue[0]=*c->active_tile;
    *c->queue_head=0;*c->queue_tail=1;
    uint32_t head=0,tail=1,processed=0;
    for(;;){
        if(++processed>50000) return 1;
        const uint32_t tile=c->queue[head],next=c->distances[tile]+1;
        for(uint32_t i=0;i<4;++i){
            const uint32_t destination=tile+(uint32_t)directions[i];
            if(c->distances[destination]!=0) continue;
            if(destination!=*c->active_tile && destination!=end){
                if(c->water_cost[destination]==255) continue;
                const uint32_t flags=c->terrain[destination];
                if((flags&0x10000)!=0 || (flags&0x40)!=0) continue;
                uint32_t j;
                for(j=0;j<8;++j){
                    const uint32_t around=destination+(uint32_t)nearby[j];
                    const uint32_t adjacent=c->terrain[around];
                    if((adjacent&0x40)!=0) break;
                    if(around==*c->active_tile || around==end) goto accept;
                    if(c->water_cost[around]==255 || (adjacent&0x10000)!=0) break;
                }
                if(j<8) continue;
            }
accept:
            c->distances[destination]=next;
            tail=*c->queue_tail;c->queue[tail]=destination;
            ++tail;*c->queue_tail=tail;
            if(signed_bits(tail)>51983){tail=0;*c->queue_tail=0;}
        }
        head=*c->queue_head+1;*c->queue_head=head;
        if(signed_bits(head)>51983) *c->queue_head=0;
        head=*c->queue_head;
        if(head==tail) return 1;
    }
}

int32_t pharaoh_map_water_path_4dir(PharaohFerryMapContext *c,int32_t route,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t variation,uint32_t *output)
{
    const uint32_t preference=*c->random_second_7&3;
    uint32_t tile=*c->origin+(uint32_t)y*228+(uint32_t)x;
    *c->active_tile=tile;
    uint32_t level=c->distances[tile],count=0,opposite=UINT32_MAX;
    if(signed_bits(level)<=0) return 0;
    while(signed_bits(level)>1){
        uint32_t current=c->distances[tile];
        const uint32_t choice=variation==1?(c->tile_random[tile]&3):preference;
        (void)pharaoh_map_general_direction(x,y,target_x,target_y);
        int32_t best=-1;
        for(uint32_t direction=0;direction<8;direction+=2){
            if(direction==opposite) continue;
            const uint32_t neighbor=c->distances[tile+(uint32_t)c->direction_offsets[direction]];
            if(neighbor!=0 && (signed_bits(neighbor)<signed_bits(current) ||
                (neighbor==current && preference==choice))){best=(int32_t)direction;current=neighbor;}
        }
        if(best==-1) return 0;
        if(best==0){y=signed_bits((uint32_t)y-1);tile-=228;}
        else if(best==2){x=signed_bits((uint32_t)x+1);++tile;}
        else if(best==4){y=signed_bits((uint32_t)y+1);tile+=228;}
        else{x=signed_bits((uint32_t)x-1);--tile;}
        opposite=(uint32_t)best+4;if(opposite>=8) opposite-=8;
        c->temporary_path[count]=(uint8_t)opposite;
        if(++count>=500) return 0;
        level=current;
    }
    if(route<1000){
        for(uint32_t i=0;i<count;++i)c->route_slots[(uint32_t)route*500+i]=c->temporary_path[count-i-1];
    }else if(output!=NULL){
        for(uint32_t i=0;i<count;++i)output[i]=c->temporary_path[count-i-1];
    }
    return (int32_t)count;
}

void pharaoh_ferry_update_tiles(PharaohFerryContext *f,int32_t building,uint8_t enabled,int32_t marker)
{
    const PharaohBuildingTypeRecord *const b=&f->buildings[building];
    const int32_t linked=b->linked_ferry_building_id;
    if(linked<=0) return;
    PharaohFerryMapContext *const c=f->map;
    const int32_t x=b->x,y=b->y;
    (void)pharaoh_map_shore_pattern(c,x,y);
    uint32_t route[1000];
    for(uint32_t i=0;i<250;++i)route[i]=UINT32_MAX;
    const int32_t link_x=f->buildings[linked].x,link_y=f->buildings[linked].y;
    const uint32_t size=b->size;
    const int32_t old_x=*c->result_x,old_y=*c->result_y;
    (void)pharaoh_map_water_tile_next_to(c,x,y,size);
    const int32_t start_x=*c->result_x,start_y=*c->result_y;
    (void)pharaoh_map_water_tile_next_to(c,link_x,link_y,size);
    const int32_t end_y=*c->result_y,end_x=*c->result_x;
    (void)pharaoh_map_ferry_water_distances(c,start_x,start_y,end_x,end_y);
    const int32_t count=pharaoh_map_water_path_4dir(c,1001,start_x,start_y,end_x,end_y,0,route);
    *c->result_x=old_x;*c->result_y=old_y;
    if(count==0) return;
    uint32_t row=*c->origin+(uint32_t)start_y*228,column=(uint32_t)start_x;
    const uint8_t cost=enabled!=0?(marker==1?1:3):255;
    uint32_t tile=row+column,flags=c->terrain[tile];
    if(enabled!=0){c->terrain[tile]=flags|0x40;c->land_routing[tile]=cost;}
    else{c->land_routing[tile]=255;c->terrain[tile]=flags&~UINT32_C(0x40);}
    for(int32_t i=0;i<count;++i){
        if(route[i]==0) row-=228;
        else if(route[i]==2) ++column;
        else if(route[i]==4) row+=228;
        else --column;
        tile=row+column;flags=c->terrain[tile];
        c->terrain[tile]=enabled!=0?flags|0x40:flags&~UINT32_C(0x40);
        c->land_routing[tile]=cost;
    }
}

static void update_scene_ferry(void *user,int32_t building,uint8_t enabled,int32_t marker)
{
    pharaoh_ferry_update_tiles(user,building,enabled,marker);
}
static void release_scene_ferry(void *user,int32_t building)
{
    pharaoh_ferry_queues_release(user,building);
}
void pharaoh_ferry_bind_scene_routing(PharaohSceneRouting *scene,PharaohFerryContext *ferry)
{
    scene->ferry_user_data=ferry;
    scene->update_ferry_tiles=update_scene_ferry;
    scene->detach_ferry=release_scene_ferry;
}
