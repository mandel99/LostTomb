#include "pharaoh/walker/chain_callbacks.h"
#include "pharaoh/core/static_objects.h"

_Static_assert(offsetof(PharaohWalkerQueryRecord,hunter_prey_id)==0x5e,"hunter target WORD");
_Static_assert(offsetof(PharaohWalkerQueryRecord,carrier_stock_index)==0x7a,"stock index BYTE");
_Static_assert(offsetof(PharaohBuildingTypeRecord,granary_food_56)==0x56,"native refunded stock");

/* Whole current/old Unit01462 C, preserving full DWORD desired type. */
int32_t pharaoh_walker_find_type_on_current_tile(const PharaohWalkerQueryRecord *walkers,
    const int16_t *heads,int32_t id,uint32_t type)
{
    int32_t target=heads[walkers[id].grid_offset];
    while(target>0){
        if((uint32_t)walkers[target].type==type)return target;
        target=walkers[target].next_walker_id_on_same_tile;
    }
    return 0;
}

static void next_frame(PharaohWalkerQueryRecord *a,uint8_t count)
{
    const uint8_t next=(uint8_t)(a->animation_frame+1u);
    a->animation_frame=next;if(next>=count)a->animation_frame=0;
}

/* Whole current/old ActorCallbacksChain C. Preserve every current-id reload. */
void pharaoh_walker_hunter_spear_tick(const PharaohWalkerChainCallbackContext *c)
{
    PharaohWalkerQueryRecord *walkers=c->motion->tile_chain->walkers;
    int32_t id=*c->current_walker_id,target,arrived,direction;
    PharaohWalkerQueryRecord *a=&walkers[id],*prey,*owner;
    a->use_cross_country=1;
    const uint8_t age=(uint8_t)((uint8_t)a->progress_on_tile+1u);
    a->progress_on_tile=(int8_t)age;if((int8_t)age>104)a->state=2;
    next_frame(a,5);
    arrived=pharaoh_walker_cross_country_advance(c->motion,id,3);
    target=pharaoh_walker_find_type_on_current_tile(walkers,c->motion->tile_chain->grid_heads,*c->current_walker_id,70);
    if(!target)target=pharaoh_walker_find_type_on_current_tile(walkers,c->motion->tile_chain->grid_heads,*c->current_walker_id,68);
    if(!target)target=pharaoh_walker_find_type_on_current_tile(walkers,c->motion->tile_chain->grid_heads,*c->current_walker_id,69);
    prey=&walkers[target];
    if(target&&prey->action_state!=12&&prey->action_state!=4){
        prey->action_state=12;prey->wait_ticks=0;a=&walkers[*c->current_walker_id];
        owner=&walkers[a->owner_walker_id];owner->hunter_prey_id=(int16_t)target;owner->action_state=11;
        owner->destination_x=prey->x;owner->destination_y=prey->y;
        /* Native event24 is ignored by the genuine47D350 zero-return body. */
        (void)pharaoh_native_return_zero_0047d350();
        id=*c->current_walker_id;walkers[id].state=2;
    }else{
        id=*c->current_walker_id;
        if(arrived){a=&walkers[id];owner=&walkers[a->owner_walker_id];owner->action_state=8;a->state=2;}
    }
    a=&walkers[id];direction=(int32_t)((uint32_t)(int32_t)(int8_t)a->direction-(*c->orientation<<2));
    *c->relative_direction=direction;
    if(direction<0){direction=(int32_t)((uint32_t)direction+32u);*c->relative_direction=direction;}
    if(!*c->rotate_spear){a->image_id=(int32_t)(*c->spear_sprite+a->animation_frame);a->cart_image_id=0;}
    else{a->image_id=(int32_t)(*c->spear_rotated_sprite+(uint32_t)direction);a->cart_image_id=(int32_t)(*c->spear_rotated_cart+(uint32_t)direction);}
}

void pharaoh_walker_carrier_tick(const PharaohWalkerChainCallbackContext *c)
{
    PharaohWalkerQueryRecord *walkers=c->motion->tile_chain->walkers;
    int32_t id=*c->current_walker_id;
    PharaohWalkerQueryRecord *a=&walkers[id];
    const int32_t previous=a->leading_walker_id,home=a->building_id;
    const int32_t root=pharaoh_delivery_boy_find_buyer(walkers,id);
    id=*c->current_walker_id;a=&walkers[id];a->is_ghost=0;a->terrain_usage=1;
    next_frame(a,12);a->cart_image_id=0;
    PharaohWalkerQueryRecord *p=&walkers[previous];
    if(previous<=0||p->action_state==4)a->state=2;
    else if(p->state!=1){
        a->state=2;
        int16_t *stock=&c->buildings[home].granary_food_56[a->carrier_stock_index];
        *stock=(int16_t)((uint16_t)*stock+100u);
    }else if(p->type!=39&&p->type!=66)a->state=2;
    else if(walkers[root].action_state!=7&&walkers[root].action_state!=6){
        pharaoh_walker_follow_chain_steps(c->motion,id,a->leading_walker_id,1);id=*c->current_walker_id;
    }
    if(walkers[root].action_state==7)walkers[id].animation_frame=0;
    if(walkers[root].action_state==6)walkers[id].is_ghost=1;
    if(p->is_ghost)walkers[id].is_ghost=1;
    a=&walkers[id];
    int32_t direction=(int8_t)a->direction<8?(int8_t)a->direction:(int8_t)a->previous_direction;
    direction=(int32_t)((uint32_t)direction-*c->orientation);*c->relative_direction=direction;
    if(direction<0){direction=(int32_t)((uint32_t)direction+8u);*c->relative_direction=direction;}
    if(a->action_state==4){
        const int32_t frame=a->wait_ticks/2;
        a->image_id=(int32_t)(*c->carrier_sprite+96u+c->corpse_frames[frame]);
    }else a->image_id=(int32_t)(*c->carrier_sprite+(uint32_t)a->animation_frame*8u+(uint32_t)direction);
}
