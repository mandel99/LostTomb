#include "pharaoh/walker/combat_steps.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/core/static_objects.h"

_Static_assert(sizeof(PharaohWalkerMovementRecord)==sizeof(PharaohWalkerQueryRecord),"same native stride");
_Static_assert(offsetof(PharaohWalkerMovementRecord,movement_x)+8==offsetof(PharaohWalkerQueryRecord,cross_country_x),"shifted movement view");
_Static_assert(offsetof(PharaohWalkerMovementRecord,movement_tick)+8==offsetof(PharaohWalkerQueryRecord,progress_on_tile),"actual tick BYTE");
_Static_assert(offsetof(PharaohFormationCountRecord,fields.ready)==0x5a,"formation ready WORD");

/* Existing whole3F00 C consumes the BCD600 view, eight bytes after query base. */
static void movement_tick(PharaohWalkerQueryRecord *walkers,int32_t id)
{
    pharaoh_walker_movement_advance_tick(
        (PharaohWalkerMovementRecord *)((uint8_t *)walkers+8),id);
}

/* Complete current/old RestoreActorFollowChainSteps algorithm. */
void pharaoh_walker_follow_chain_steps(const PharaohWalkerMotionContext *c,
    int32_t id,int32_t predecessor_id,int32_t steps)
{
    PharaohWalkerQueryRecord *a=&c->tile_chain->walkers[id],*p=&c->tile_chain->walkers[predecessor_id];
    if(a->x==a->source_x){if(a->y==a->source_y)a->is_ghost=1;}
    while(steps>0){
        --steps;
        const uint8_t timer=(uint8_t)((uint8_t)a->progress_on_tile+1u);
        a->progress_on_tile=(int8_t)timer;
        if((int8_t)timer>=15){
            a->progress_on_tile=15;
            const uint8_t direction=(uint8_t)pharaoh_map_general_direction(a->x,a->y,p->previous_x,p->previous_y);
            a->direction=direction;
            if((int8_t)direction>=8)return;
            a->previous_direction=direction;a->progress_on_tile=0;
        }
        movement_tick(c->tile_chain->walkers,id);
        if(a->progress_on_tile==8)pharaoh_walker_move_to_next_tile(c,id);
    }
}

/* Whole current/old mixed Unit01414 melee C; preserve wrapped DWORD math. */
void pharaoh_walker_melee_tick(const PharaohWalkerMeleeContext *context,int32_t id)
{
    const PharaohWalkerContactContext *c=context->contact;
    PharaohWalkerQueryRecord *a=&c->walkers[id],*t;
    const int32_t formation=a->formation_id_60;
    int32_t target,target_formation,attack,defense,bonus,limit,damage,kind,count;
    uint8_t mode,tick,total;
    int16_t first;
    if(a->progress_on_tile<=5){
        a->progress_on_tile=(int8_t)((uint8_t)a->progress_on_tile+1u);
        movement_tick(c->walkers,id);
    }
    mode=a->combat_flag_08b;
    if(!mode){pharaoh_walker_combat_resume_activity_after_attack(c->walkers,context->slot_walkers,id);return;}
    target=a->combat_value_090;t=&c->walkers[target];
    if(mode==1){
        if(t->state!=1||t->action_state==4){pharaoh_walker_combat_resume_activity_after_attack(c->walkers,context->slot_walkers,id);return;}
    }else if(mode==2&&(t->state!=1||t->action_state==4)){
        first=a->combat_value_08c;
        if(target==first)a->combat_value_090=a->combat_value_08e;
        else if(target==a->combat_value_08e)a->combat_value_090=first;
        target=a->combat_value_090;t=&c->walkers[target];
        if(t->state!=1||t->action_state==4){pharaoh_walker_combat_resume_activity_after_attack(c->walkers,context->slot_walkers,id);return;}
        a->combat_flag_08b=1;a->combat_value_08c=(int16_t)target;a->combat_value_08e=0;
    }
    tick=(uint8_t)(a->combat_delay+1u);a->combat_delay=tick;
    if(tick<24)return;
    target=a->combat_value_090;t=&c->walkers[target];target_formation=t->formation_id_60;
    kind=c->properties[t->type].category;a->combat_delay=(uint8_t)(kind==1||kind==4?12:0);
    PharaohNativeLegionRecord *af=&c->formations[formation],*tf;
    bonus=(uint32_t)t->owner==(uint32_t)*c->current_player?((int32_t)af->strength+10)/20:0;
    attack=c->properties[a->type].attack;
    defense=(int32_t)((uint32_t)c->properties[t->type].defense+(uint32_t)bonus);
    if(defense>=20)defense=20;
    if(t->combat_value_090!=id&&pharaoh_direction_is_same_or_adjacent((int8_t)a->direction,(int8_t)t->direction)){
        kind=af->strength_class;
        if(kind==12||kind==13){
            attack=(int32_t)((uint32_t)attack+4u);
            /* Native event141 reaches the genuine47D350 zero-return body. */
            (void)pharaoh_native_return_zero_0047d350();
        }
    }
    if(c->formation_counts->formations[formation].fields.ready){
        kind=af->strength_class;
        if(kind==12){
            if(pharaoh_direction_is_same_or_adjacent((int8_t)a->direction,af->direction)){
                count=af->movement_mode;
                if(count>0){if(count<=2)attack=(int32_t)((uint32_t)attack+4u);else if(count==6)attack=(int32_t)((uint32_t)attack+2u);}
            }
        }else if(kind==11||kind==13){
            if(pharaoh_direction_is_same_or_adjacent((int8_t)a->direction,af->direction)&&af->movement_mode==6)
                attack=(int32_t)((uint32_t)attack+2u);
        }
    }
    tf=&c->formations[target_formation];
    if(c->formation_counts->formations[target_formation].fields.ready){
        kind=tf->strength_class;
        if(kind==12){
            if(pharaoh_direction_is_same_or_adjacent((int8_t)t->direction,tf->direction)){
                count=tf->movement_mode;
                if(count>0){if(count<=2)defense=(int32_t)((uint32_t)defense+4u);else if(count==6)defense=(int32_t)((uint32_t)defense-2u);}
            }else defense=(int32_t)((uint32_t)defense-4u);
        }else if(kind==11||kind==13){
            if(pharaoh_direction_is_same_or_adjacent((int8_t)t->direction,tf->direction)){
                if(tf->movement_mode==6)defense=(int32_t)((uint32_t)defense-2u);
            }else defense=(int32_t)((uint32_t)defense-4u);
        }
    }
    if(af->strength_class==57&&pharaoh_direction_is_same_or_adjacent((int8_t)a->direction,af->direction)){
        count=af->movement_mode;if(count>0&&count<=2)attack=(int32_t)((uint32_t)attack+4u);
    }
    if(tf->strength_class==57&&pharaoh_direction_is_same_or_adjacent((int8_t)t->direction,tf->direction)){
        count=tf->movement_mode;if(count>0&&count<=2)defense=(int32_t)((uint32_t)defense+4u);
    }
    limit=c->properties[t->type].maximum_damage;
    if(defense<0)defense=0;if(defense>20)defense=20;
    damage=attack?(int32_t)((20u-(uint32_t)defense)*(uint32_t)attack)/20:0;
    if(damage<0)damage=0;
    total=(uint8_t)(t->damage+(uint8_t)damage);t->damage=total;
    if((int32_t)total<=limit){pharaoh_walker_combat_sound(c->sound,id);return;}
    if(!(uint8_t)pharaoh_building_type_is_004ea9c0_group(t->type)&&
       !(uint8_t)pharaoh_walker_type_is_herd_animal(t->type)&&t->type!=23&&
       (uint32_t)a->owner==(uint32_t)*c->current_player){
        bonus=(int32_t)af->strength+1;if(bonus>=100)bonus=100;af->strength=(int16_t)bonus;
    }
    t->action_state=4;t->wait_ticks=0;
    pharaoh_walker_kind_sound(c,target);pharaoh_formation_contact_loss(c,target_formation);
}
