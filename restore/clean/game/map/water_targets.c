#include "pharaoh/map/native_leaf_queries.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohNativeDockPlayer)==0x49d8,"native player stride");
_Static_assert(sizeof(PharaohWalkerQueryRecord)==388,"walker stride");
_Static_assert(offsetof(PharaohWalkerQueryRecord,reserved_07a)==0x7a,"dock cursor BYTE");
_Static_assert(offsetof(PharaohBuildingTypeRecord,industry_orientation)==0xaa,"dock orientation BYTE");
_Static_assert(offsetof(PharaohBuildingTypeRecord,runtime_value_b4)==0xb4,"dock ship WORD");

static uint32_t difference(int32_t a,int32_t b)
{return a>b?(uint32_t)a-(uint32_t)b:(a<b?(uint32_t)b-(uint32_t)a:0u);}

int32_t pharaoh_map_distance_with_penalty(const PharaohWaterTargetContext *c,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t region1,int32_t region2)
{
    uint32_t penalty=difference(region1,region2);
    if(region1==-1)penalty=0;
    const uint32_t dx=difference(x1,x2);*c->difference_x=dx;
    const uint32_t dy=difference(y1,y2);*c->difference_y=dy;
    return (int32_t)(((int32_t)dx<(int32_t)dy?dy:dx)+penalty);
}

int32_t pharaoh_map_find_permitted_neighbor(const PharaohWaterTargetContext *c,int32_t x,int32_t y,int32_t row)
{
    const uint32_t base=*c->origin+(uint32_t)y*228u+(uint32_t)x;
    for(uint32_t i=0;i<24;++i){
        const uint32_t delta=(uint32_t)c->perimeter_offsets[row][i];
        if(delta==0)return 0;
        const uint32_t flags=c->terrain[base+delta]&0xeefed77fu;
        if((flags&0x40u)!=0||flags==0){
            const uint32_t px=(uint32_t)c->coordinate_offsets[row][i][0]+(uint32_t)x;
            const uint32_t py=(uint32_t)c->coordinate_offsets[row][i][1]+(uint32_t)y;
            *c->result_x=px;*c->result_y=py;return 1;
        }
    }
    return 0;
}

int32_t pharaoh_dock_claim_position(const PharaohWaterTargetContext *c,int32_t id)
{
    int32_t candidate=0;
    if(*c->dock_count<=0)return 0;
    uint32_t index=c->walkers[id].reserved_07a;
    if(index>=10)return 0;
    const volatile int16_t *list=&c->players[*c->current_player].dock_ids[index];
    do{
        candidate=*list;
        if(candidate>0){
            const int16_t owner=c->buildings[candidate].runtime_value_b4;
            if(owner==0||owner==id)break;
        }
        ++index;++list;
    }while(index<10);
    /* Native accepts the last positive candidate after occupied exhaustion. */
    if(candidate<=0)return 0;
    volatile PharaohBuildingTypeRecord *b=&c->buildings[candidate];
    const uint8_t orientation=b->industry_orientation;
    uint32_t x=(uint32_t)(int32_t)b->x;
    x+=orientation==1?3u:(orientation==0||orientation==2?1u:UINT32_MAX);
    *c->result_x=x;
    /* Native captures Y only after X publication. */
    uint32_t y=(uint32_t)(int32_t)b->y;
    y+=orientation==0?UINT32_MAX:(orientation==2?3u:1u);
    b->runtime_value_b4=(int16_t)id;*c->result_y=y;
    return candidate;
}

typedef struct WaterPoint { uint32_t x,y; } WaterPoint;

static WaterPoint publish_point(const PharaohWaterTargetContext *c,int32_t id,uint32_t negative,uint32_t positive)
{
    const volatile PharaohBuildingTypeRecord *b=&c->buildings[id];
    const uint8_t orientation=b->industry_orientation;
    WaterPoint p={(uint32_t)(int32_t)b->x,(uint32_t)(int32_t)b->y};
    p.x+=orientation==1?positive:(orientation==0||orientation==2?1u:0u-negative);
    p.y+=orientation==0?0u-negative:(orientation==2?positive:1u);
    *c->result_x=p.x;*c->result_y=p.y;return p;
}

int32_t pharaoh_dock_find_waiting_position(const PharaohWaterTargetContext *c,int32_t id)
{
    if(*c->dock_count<=0)return 0;
    uint32_t base=*c->origin,start=c->walkers[id].reserved_07a,player=*c->current_player;
    if(start<10){
        const volatile int16_t *list=&c->players[player].dock_ids[start];
        for(uint32_t i=start;i<10;++i,++list){
            const int32_t candidate=*list;if(candidate<=0)continue;
            const WaterPoint p=publish_point(c,candidate,2,4);
            base=*c->origin;
            if(c->walker_heads[base+p.y*228u+p.x]==0)return candidate;
            player=*c->current_player;
        }
        /* Fallback borrows the last cached player/base, not a fresh global. */
        list=&c->players[player].dock_ids[start];
        for(uint32_t i=start;i<10;++i,++list){
            const int32_t candidate=*list;if(candidate<=0)continue;
            const WaterPoint p=publish_point(c,candidate,3,5);
            if(c->walker_heads[base+p.y*228u+p.x]==0)return candidate;
        }
    }
    return 0;
}

int32_t pharaoh_water_find_available_building(const PharaohWaterTargetContext *c,int32_t id)
{
    for(int32_t building=1;building<4000;++building){
        const volatile PharaohBuildingTypeRecord *b=&c->buildings[building];
        if(b->state!=1)continue;
        const int16_t type=b->building_type;
        if(type!=76&&type!=181&&type!=182)continue;
        const int16_t owner=b->runtime_value_b4;
        if(owner!=0&&owner!=id)continue;
        (void)publish_point(c,building,1,2);return building;
    }
    return 0;
}

int32_t pharaoh_water_find_nearest_waypoint(const PharaohWaterTargetContext *c,int32_t id)
{
    uint32_t count=0,chosen=0;
    int32_t best=10000;
    for(uint32_t i=0;i<8;++i)if(c->point_x[i]>0)++count;
    if(count==0)return 0;
    for(uint32_t i=0;i<8;++i){
        const int16_t raw_x=c->point_x[i];if(raw_x<=0)continue;
        const int32_t px=raw_x,py=c->point_y[i],y=c->walkers[id].y,x=c->walkers[id].x;
        const uint32_t dx=difference(x,px);*c->difference_x=dx;
        const uint32_t dy=difference(y,py);*c->difference_y=dy;
        const int32_t distance=(int32_t)((int32_t)dx<(int32_t)dy?dy:dx);
        if(distance<best){best=distance;chosen=i;}
    }
    if(best>=10000)return 0;
    /* Exact BYTE loads from WORD coordinates, not widened WORD reads. */
    const uint8_t x=*(const volatile uint8_t *)&c->point_x[chosen];
    const uint8_t y=*(const volatile uint8_t *)&c->point_y[chosen];
    *c->result_x=x;*c->result_y=y;return 1;
}

static uint8_t terrain_low(const PharaohWaterTargetContext *c,uint32_t tile)
{return *(const volatile uint8_t *)&c->terrain[tile];}

/* Whole current/old two radius-five water scans, with distinct initial gates. */
static int32_t water_search(const PharaohWaterTargetContext *c,int32_t id,int cross)
{
    uint32_t tile=(uint32_t)c->walkers[id].grid_offset;
    if(cross){if((terrain_low(c,tile)&4u)!=0&&c->walker_heads[tile]==id)return 0;}
    else if(c->walker_heads[tile]==id)return 1;
    const int32_t x=c->walkers[id].x,y=c->walkers[id].y;
    for(uint32_t radius=1;radius<=5;++radius){
        uint32_t left=(uint32_t)x-radius,right=(uint32_t)x+radius;
        uint32_t top=(uint32_t)y-radius,bottom=(uint32_t)y+radius;
        if((int32_t)left<0)left=0;if((int32_t)top<0)top=0;
        const uint32_t width=(uint32_t)*c->width,height=(uint32_t)*c->height;
        if((int32_t)right>=(int32_t)width)right=width-1u;
        if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
        uint32_t base;
        if(cross){*c->result_y=top;base=*c->origin;}
        else{base=*c->origin;*c->result_y=top;}
        tile=base+top*228u+left;
        const uint32_t row_skip=left-right+227u;
        uint32_t row=top;
        while((int32_t)row<=(int32_t)bottom){
            uint32_t column=left;*c->result_x=column;
            while((int32_t)column<=(int32_t)right){
                if((terrain_low(c,tile)&4u)!=0){
                    const int32_t head=c->walker_heads[tile];
                    if(!cross){if(head==0)return 0;}
                    else if((head==0||head==id)&&(terrain_low(c,tile-456u)&4u)!=0&&
                        (terrain_low(c,tile+2u)&4u)!=0&&(terrain_low(c,tile+456u)&4u)!=0&&
                        (terrain_low(c,tile-2u)&4u)!=0)return 1;
                }
                ++column;++tile;*c->result_x=column;
            }
            row=*c->result_y+1u;tile+=row_skip;*c->result_y=row;
        }
    }
    return cross?0:1;
}

int32_t pharaoh_water_space_blocked(const PharaohWaterTargetContext *c,int32_t id)
{return water_search(c,id,0);}
int32_t pharaoh_water_find_unoccupied_cross(const PharaohWaterTargetContext *c,int32_t id)
{return water_search(c,id,1);}

uint32_t pharaoh_map_no_fighting_enemy(const PharaohWaterTargetContext *c,int32_t tile)
{
    int32_t id=c->walker_heads[tile];if(id<=0)return 1;
    do{
        if(c->walkers[id].reserved_00d==0&&c->walkers[id].action_state==5)return 0;
        id=c->walkers[id].next_walker_id_on_same_tile;
    }while(id!=0);
    return 1;
}

uint32_t pharaoh_map_no_fighting_friendly(const PharaohWaterTargetContext *c,int32_t tile)
{
    int32_t id=c->walker_heads[tile];if(id<=0)return 1;
    do{
        if(c->walkers[id].reserved_00d==1&&c->walkers[id].action_state==5)return 0;
        const int32_t next=c->walkers[id].next_walker_id_on_same_tile;
        if(id==next)return 1;id=next;
    }while(id!=0);
    return 1;
}
