#include "pharaoh/map/bridge_placement.h"

/* Only recognized native directions write a piece byte. In particular an
 * unsupported direction keeps the old byte, except the unconditional ship
 * segment13 at indices1 andlength-2. */
static void bridge_piece(volatile uint8_t *destination,uint32_t direction,
    int32_t mode,uint32_t index,uint32_t length,uint32_t middle)
{
    static const uint8_t low_first[4] = {4,1,6,3};
    static const uint8_t low_last[4] = {6,3,4,1};
    static const uint8_t low_inside[4] = {5,2,5,2};
    static const uint8_t ship_first[4] = {7,8,9,10};
    static const uint8_t ship_last[4] = {9,10,7,8};
    static const uint8_t ship_middle[4] = {14,15,14,15};
    static const uint8_t ship_inside[4] = {11,12,11,12};
    const uint8_t *pieces;

    if(mode==0){
        if(index==length-1u)pieces=low_last;
        else if(index==0)pieces=low_first;
        else pieces=low_inside;
    }else{
        /* These tests precede endpoint and middle selection natively. */
        if(index==length-2u || index==1u){*destination=13;return;}
        if(index==length-1u)pieces=ship_last;
        else if(index==0)pieces=ship_first;
        else if(index==middle)pieces=ship_middle;
        else pieces=ship_inside;
    }
    if(direction<=6u && (direction&1u)==0)
        *destination=pieces[direction>>1];
}

void pharaoh_map_place_bridge(const PharaohBridgePlacementContext *c,
    int32_t x,int32_t y,int32_t mode)
{
    PharaohBridgePlacementState *const state=c->state;
    if(state->available<=0){state->length=0;return;}
    const int32_t length=state->length;
    if(length<=1){state->length=0;return;}
    if(mode==1 && length<5){state->length=0;return;}

    const uint32_t count=(uint32_t)length;
    uint32_t tile=(uint32_t)c->routing->geometry->grid_start+
        (uint32_t)y*228u+(uint32_t)x;
    uint32_t direction=state->direction-
        (uint32_t)*c->routing->map_orientation;
    state->direction=direction;
    /* Native adds8 only once when the wrapped subtraction has its signbit. */
    if((int32_t)direction<0){direction+=8u;state->direction=direction;}

    uint32_t middle=0;
    if(mode==1){
        if(count==9u || count==10u)middle=4;
        else if(count==11u || count==12u)middle=5;
        else if(count==13u || count==14u)middle=6;
        else if(count==15u || count==16u)middle=7;
        else middle=8;
    }
    const uint32_t step=state->tile_step;
    volatile uint32_t *const terrain=c->routing->terrain;
    volatile uint8_t *const directions=c->tile_direction;
    for(uint32_t index=0;index<count;++index){
        /* Whole DWORD OR and conditional BYTE store, including step0. */
        terrain[tile]|=0x40u;
        bridge_piece(&directions[tile],direction,mode,index,count,middle);
        tile+=step;
    }
    pharaoh_map_routing_initialize_water(c->routing);
    pharaoh_map_routing_initialize_citizen(c->routing);
    pharaoh_map_routing_initialize_noncitizen(c->routing);
}
