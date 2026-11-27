#include "pharaoh/walker/contact.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/map/terrain_queries.h"

_Static_assert(sizeof(PharaohWalkerContactPlayerRecord)==0x49d8,"native profile stride");
_Static_assert(offsetof(PharaohWalkerContactPlayerRecord,native_ea5ce8)==0x60,"native profile WORD");
_Static_assert(offsetof(PharaohWalkerContactPlayerRecord,building_ea5f9c)==0x314,"native profile DWORD");
_Static_assert(offsetof(PharaohWalkerQueryRecord,combat_action_counter_134)==0x134,"contact counter");
_Static_assert(sizeof(PharaohWalkerTypeProperties)==56,"native model stride");
_Static_assert(sizeof(PharaohNativeLegionRecord)==144,"native formation stride");
_Static_assert(offsetof(PharaohNativeLegionRecord,morale)==0x2c,"native morale");
_Static_assert(offsetof(PharaohNativeLegionRecord,morale_leader_id)==0x48,"native morale gate");
_Static_assert(offsetof(PharaohNativeLegionRecord,strength)==0x7a,"native strength");

static int32_t random_cue(uint32_t sample,uint32_t shift,int32_t first)
{
    return first+(int32_t)(sample<<shift)/32768;
}

/* Whole5050 current/old ActorCommand C. Every real rand call updates its seed. */
void pharaoh_walker_kind_sound(const PharaohWalkerContactContext *c,int32_t id)
{
    int32_t sound;
    switch(c->walkers[id].type){
    case 13:case 45:case 56:sound=13;break;
    case 26:sound=random_cue(pharaoh_crt_random_next(c->random),2,29);break;
    case 68:sound=8;break;
    case 69:sound=4;break;
    case 70:sound=5;break;
    case 77:case 78:case 92:case 93:case 100:case 101:sound=17;break;
    case 82:sound=7;break;
    case 83:sound=0;break;
    case 84:sound=6;break;
    case 102:sound=3;break;
    case 103:sound=1;break;
    case 104:sound=2;break;
    default:sound=random_cue(pharaoh_crt_random_next(c->random),3,21);break;
    }
    pharaoh_walker_sound_play(c->sound,id,sound,3);
}

void pharaoh_formation_contact_loss(const PharaohWalkerContactContext *c,int32_t id)
{
    int16_t penalty;
    if(id==0)return;
    pharaoh_formation_calculate_figures(c->formation_counts);
    const uint8_t members=c->formation_counts->formations[id].fields.member_count;
    /* Numerator1, denominator BYTE0..255: real percentage status is always VALUE. */
    const int32_t percentage=pharaoh_scale_value_to_percent(1,members).value;
    if(percentage<8)penalty=-5;
    else if(percentage<10)penalty=-7;
    else if(percentage<14)penalty=-10;
    else if(percentage<20)penalty=-12;
    else if(percentage<30)penalty=-15;
    else penalty=-20;
    pharaoh_formation_change_morale_native(c->formations,id,penalty);
}

void pharaoh_walker_contact_damage(const PharaohWalkerContactContext *c,int32_t id,int32_t target_id)
{
    PharaohWalkerQueryRecord *a=&c->walkers[id],*t=&c->walkers[target_id];
    const int32_t group=a->formation_id_60,target_group=t->formation_id_60;
    uint32_t power,defense,product,damage=0;
    int32_t counter;
    uint8_t kind,accumulated;
    if(id==target_id)return;
    if((uint32_t)t->owner==(uint32_t)*c->current_player)return;
    kind=a->type;counter=a->combat_action_counter_134;
    power=(uint32_t)c->properties[kind].attack;
    defense=(uint32_t)c->properties[kind].defense;
    if(counter>5&&counter<20)power*=4u;
    if(power!=0){
        product=(uint32_t)((int32_t)(30u-defense)/20)*power;
        if((int32_t)product>=0)damage=product;
    }
    accumulated=(uint8_t)((uint32_t)t->damage+(damage&255u));t->damage=accumulated;
    kind=t->type;
    if((int32_t)accumulated<=c->properties[kind].maximum_damage){
        pharaoh_walker_combat_sound(c->sound,id);return;
    }
    if((uint8_t)pharaoh_building_type_is_004ea9c0_group(kind)==0){
        if((uint8_t)pharaoh_walker_type_is_herd_animal(t->type)==0&&t->type!=23){
            if((uint32_t)a->owner==(uint32_t)*c->current_player){
                int32_t value=(int32_t)c->formations[group].strength+1;
                if(value>=100)value=100;
                c->formations[group].strength=(int16_t)value;
            }
        }
    }
    t->action_state=4;t->wait_ticks=0;
    pharaoh_walker_kind_sound(c,target_id);
    pharaoh_formation_contact_loss(c,target_group);
}

/* Both separately authenticated77-byte remaps share these seven exclusions. */
static int contact_kind_skipped(uint8_t kind)
{
    switch(kind){case 25:case 77:case 78:case 92:case 93:case 100:case 101:return 1;default:return 0;}
}

static void begin_pair(int32_t id,int32_t target_id,
    PharaohWalkerQueryRecord *a,PharaohWalkerQueryRecord *t)
{
    uint16_t target_x,target_y;
    uint8_t direction,state,queue;
    int32_t x,y,tx,ty;
    a->action_state_before_6_7=a->action_state;a->action_state=5;
    a->combat_value_090=(int16_t)target_id;a->combat_value_08c=(int16_t)target_id;
    a->combat_flag_08b=1;a->combat_delay=12;
    target_x=(uint16_t)t->x;
    if(target_x==(uint16_t)t->destination_x){
        target_y=(uint16_t)t->y;
        if(target_y==(uint16_t)t->destination_y){
            tx=(int16_t)target_x;ty=(int16_t)target_y;x=a->previous_x;y=a->previous_y;
        }else{tx=t->previous_x;ty=t->previous_y;x=a->previous_x;y=a->previous_y;}
    }else{tx=t->previous_x;ty=t->previous_y;x=a->previous_x;y=a->previous_y;}
    direction=(uint8_t)pharaoh_map_general_direction(x,y,tx,ty);a->direction=direction;
    if((int8_t)direction>=8)a->direction=0;
    state=t->action_state;
    if(state!=5){
        t->action_state_before_6_7=state;t->action_state=5;t->combat_delay=0;
        direction=(uint8_t)(a->direction+4u);t->direction=direction;
        if((int8_t)direction>=8)t->direction=(uint8_t)(direction-8u);
    }
    queue=t->combat_flag_08b;
    if(queue==0){t->combat_value_08c=(int16_t)id;t->combat_value_090=(int16_t)id;t->combat_flag_08b=1;}
    else if(queue==1){t->combat_value_08e=(int16_t)id;t->combat_flag_08b=2;}
}

void pharaoh_walker_contact_chain(const PharaohWalkerContactContext *c,int32_t id,int32_t candidate)
{
    PharaohWalkerQueryRecord *a=&c->walkers[id],*t;
    const int32_t own_class=c->properties[a->type].category;
    uint32_t steps=1;
    int eligible=0;
    if(own_class<=0||own_class>=8)return;
    const uint8_t own_state=a->action_state;
    if(own_state==19){pharaoh_walker_contact_damage(c,id,candidate);return;}
    t=&c->walkers[candidate];
    if(t->action_state==19||own_state==5)return;
    while(candidate>0){
        if(candidate==id)candidate=a->next_walker_id_on_same_tile;
        else{
            t=&c->walkers[candidate];
            if(!contact_kind_skipped(t->type)){
                const int32_t target_class=c->properties[t->type].category;
                if(t->state!=1||t->action_state==4)eligible=0;
                else switch(own_class){
                case 2:if(target_class==5){if(t->action_state==11)eligible=1;}else if(target_class==4||target_class==3||target_class==7)eligible=1;break;
                case 3:if(target_class==1||target_class==2||target_class==4||target_class==7)eligible=1;break;
                case 4:if(target_class==2||target_class==3||target_class==7)eligible=1;break;
                case 1:case 6:if(target_class==7)eligible=1;break;
                case 7:if(target_class==1||target_class==2||target_class==3||target_class==4||target_class==6)eligible=1;break;
                default:break;
                }
                const uint8_t own_kind=a->type;
                if(own_kind==23&&t->type==4&&t->resource_id==21){
                    a->action_state=10;a->wait_ticks=0;t->state=2;
                    PharaohWalkerContactPlayerRecord *p=&c->players[*c->current_player];
                    p->counter_ea5c88+=1u;return;
                }
                if(own_kind==4&&a->resource_id==21&&t->type==23){t->action_state=10;t->wait_ticks=0;a->state=2;return;}
                if(own_kind==33&&t->type==98){t->state=2;return;}
                if(own_kind==98&&t->type==33){a->state=2;return;}
                PharaohWalkerContactPlayerRecord *p=&c->players[*c->current_player];
                if(p->native_ea5ce8!=0){
                    if(pharaoh_building_group_index(c->buildings,p->building_ea5f9c)==4){
                        p=&c->players[*c->current_player];
                        if(pharaoh_temple_upgrade_present(c->buildings,p->building_ea5f9c,1)){
                            if(a->type==27&&pharaoh_building_group_index(c->buildings,a->building_id)==4){if(t->type==98){t->state=2;return;}}
                            if(a->type==98&&t->type==27){if(pharaoh_building_group_index(c->buildings,t->building_id)==4){a->state=2;return;}}
                        }
                    }
                }
                if(eligible){
                    if(t->action_state!=5||t->combat_flag_08b<2){begin_pair(id,candidate,a,t);return;}
                    eligible=0;
                }
            }
            candidate=t->next_walker_id_on_same_tile;
        }
        ++steps;if(steps>=1000)return;
    }
}

void pharaoh_walker_scan_neighbor_contacts(const PharaohWalkerContactContext *c,int32_t id)
{
    PharaohWalkerQueryRecord *a=&c->walkers[id];
    const uint32_t tile=(uint32_t)a->grid_offset;
    for(uint32_t i=0;i<8;++i){
        if(a->action_state==5)break;
        const int32_t delta=c->adjacent_grid_offsets[i];
        if(contact_kind_skipped(a->type))continue;
        pharaoh_walker_contact_chain(c,id,c->grid_heads[tile+(uint32_t)delta]);
    }
}
