#include "pharaoh/building/house_health.h"
#include "pharaoh/core/x87_numeric.h"

static uint32_t distance_to(const PharaohBuildingTypeRecord *b,int32_t x,int32_t y)
{
    const uint32_t dx=(uint32_t)(int32_t)b->x-(uint32_t)x;
    const uint32_t dy=(uint32_t)(int32_t)b->y-(uint32_t)y;
    return (uint32_t)pharaoh_x87_sqrt_truncate_i32((int32_t)(dx*dx+dy*dy));
}

/* Completed prior BuildingQualityUpdate C, native004E7F00. */
void pharaoh_house_update_health(PharaohHouseHealthContext *c,
    int32_t building,uint32_t recalculate)
{
    uint32_t player=*c->current_player,water,road,bonus=16,divisor=1,value;
    PharaohBuildingTypeRecord *const b=&c->buildings[building];
    pharaoh_random_generator_advance(&c->random);
    value=(uint32_t)b->house_plague_risk+(uint32_t)c->house_models[b->subtype].disease_risk+
        (50u-(uint32_t)c->players[player].health_rating)*2u;
    b->house_plague_risk=(int32_t)value;
    if ((int32_t)value<0) b->house_plague_risk=0;
    if (b->house_plague_risk>1000) b->house_plague_risk=1000;
    if ((uint8_t)recalculate) {
        int32_t x,y,radius=c->search.width;
        if (c->search.height>radius) radius=c->search.height;
        water=pharaoh_map_ring_actor_search(&c->search,b->x,b->y,0,0,0x40000,
            &x,&y,radius,0)?distance_to(b,x,y):(uint32_t)radius;
        road=pharaoh_map_ring_actor_search(&c->search,b->x,b->y,0,0,4,
            &x,&y,radius,0)?distance_to(b,x,y):(uint32_t)radius;
        player=*c->current_player;
        b->health_distance_4=(int32_t)road;b->health_distance_40000=(int32_t)water;
    } else {
        water=(uint32_t)b->health_distance_40000;road=(uint32_t)b->health_distance_4;
    }
    if (*c->difficulty==0) road=1;
    else if (*c->difficulty==2) {bonus=8;divisor=2;}
    value=(uint32_t)c->house_models[b->subtype].malaria_risk-
        (uint32_t)c->players[player].health_rating+(55u-water)*2u-road+bonus;
    value=(uint32_t)b->house_malaria_risk+(uint32_t)((int32_t)value/(int32_t)divisor);
    b->house_malaria_risk=(int32_t)value;
    if ((int32_t)value<0) b->house_malaria_risk=0;
    if (b->house_malaria_risk>1000) b->house_malaria_risk=1000;
}
