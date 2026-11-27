#include "pharaoh/military/formation_counts.h"
#include "pharaoh/military/transport_boats.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/core/memory_fill.h"
#include "pharaoh/core/static_objects.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohFormationCountRecord)==144 &&
    offsetof(PharaohFormationCountRecord,fields.members)==8 &&
    offsetof(PharaohFormationCountRecord,fields.member_count)==40 &&
    offsetof(PharaohFormationCountRecord,fields.flag_42)==66 &&
    offsetof(PharaohFormationCountRecord,fields.damage_total)==74 &&
    offsetof(PharaohFormationCountRecord,fields.model_total)==76 &&
    offsetof(PharaohFormationCountRecord,fields.ready)==90 &&
    offsetof(PharaohFormationCountRecord,fields.flag_67)==103 &&
    offsetof(PharaohFormationCountRecord,fields.skip_69)==105 &&
    offsetof(PharaohFormationCountRecord,fields.event_id)==120,
    "native formation fields");
_Static_assert(PHARAOH_WALKER_RECORD_COUNT==2000 &&
    sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,type)==10 &&
    offsetof(PharaohWalkerQueryRecord,state)==14 &&
    offsetof(PharaohWalkerQueryRecord,direction)==17 &&
    offsetof(PharaohWalkerQueryRecord,damage)==29 &&
    offsetof(PharaohWalkerQueryRecord,action_state)==56 &&
    offsetof(PharaohWalkerQueryRecord,formation_id_60)==96 &&
    sizeof(PharaohFormationUnitModel)==56 && sizeof(PharaohScenarioEvent)==124,
    "native recount input strides");

static int eligible(const PharaohWalkerQueryRecord *walker)
{
    uint32_t kind=walker->type;
    if((kind>=11 && kind<=13)||(kind>=43 && kind<=53)||kind==57) return 1;
    if((uint8_t)pharaoh_walker_type_is_herd_animal((int32_t)kind)!=0) return 1;
    if((uint8_t)pharaoh_building_type_is_004ea9c0_group(walker->type)!=0)
        return 1;
    kind=walker->type;
    return kind==99 || kind==54 || kind==55 || kind==56;
}

void pharaoh_formation_calculate_figures(const PharaohFormationCountContext *c)
{
    for(int32_t id=1;id<50;++id){
        PharaohFormationCountRecord *const formation=&c->formations[id];
        for(int32_t i=0;i<8;++i) formation->words[2+i]=0;
        formation->fields.member_count=0;
        formation->fields.all_members_flag=1;
        formation->fields.damage_total=0;
        formation->fields.model_total=0;
    }
    for(int32_t id=1;id<PHARAOH_WALKER_RECORD_COUNT;++id){
        PharaohWalkerQueryRecord *const walker=&c->walkers[id];
        if(walker->state!=1 || walker->action_state==4 || !eligible(walker))
            continue;
        PharaohFormationCountRecord *const formation=
            &c->formations[walker->formation_id_60];
        ++formation->fields.member_count;
        formation->fields.model_total=(uint16_t)(formation->fields.model_total+
            c->models[walker->type].value);
        formation->fields.damage_total=(uint16_t)(
            formation->fields.damage_total+walker->damage);
        if(((const uint8_t *)walker)[0x63]!=1)
            formation->fields.all_members_flag=0;
        for(int32_t slot=0;slot<16;++slot) if(formation->fields.members[slot]==0){
            ((uint8_t *)walker)[0x62]=(uint8_t)slot;
            formation->fields.members[slot]=(int16_t)id;
            break;
        }
    }
    c->totals[3]=0;c->totals[2]=0;c->totals[1]=0;c->totals[0]=0;
    int32_t removed_count=0;
    for(int32_t id=1;id<50;++id){
        PharaohFormationCountRecord *const formation=&c->formations[id];
        if(formation->fields.state!=1 || formation->fields.skip_69!=0) continue;
        if(formation->fields.flag_42==0){
            if(formation->fields.member_count!=0){
                ++c->totals[3];
                c->totals[2]+=formation->fields.member_count;
            }else{
                const int32_t event=formation->fields.event_id;
                int32_t i=0;
                while(i<removed_count && c->event_ids[i]!=event) ++i;
                if(i==removed_count){
                    c->event_ids[removed_count]=event;
                    c->event_flags[removed_count]=1;
                    ++removed_count;
                }
                pharaoh_memory_fill_bytes(formation->words,144,0);
            }
        }else if(formation->fields.member_count!=0){
            const uint16_t previous_ready=formation->fields.ready;
            formation->fields.ready=1;
            for(int32_t i=0;i<formation->fields.member_count;++i){
                /* Native count is not clamped to the16 roster slots. */
                int16_t walker;
                memcpy(&walker,(const uint8_t *)c->formations+
                    (size_t)id*144+8+(size_t)i*2,sizeof(walker));
                if(walker!=0 && c->walkers[walker].direction!=8)
                    formation->fields.ready=0;
            }
            ++c->totals[1];
            c->totals[0]+=formation->fields.member_count;
            if(formation->fields.type==13){
                if(previous_ready==0 && formation->fields.ready==1)
                    (void)pharaoh_native_return_zero_0047d350();
                c->totals[0]+=formation->fields.member_count>>1;
            }
        }
    }
    static const uint8_t reset_order[7]={8,10,7,5,6,0,1};
    for(int32_t i=0;i<7;++i) c->player_counters[reset_order[i]]=0;
    for(int32_t id=1;id<50;++id){
        PharaohFormationCountRecord *const formation=&c->formations[id];
        if(formation->fields.state!=1) continue;
        if(formation->fields.flag_42==0){
            const int32_t event=formation->fields.event_id;
            for(int32_t i=0;i<removed_count;++i)
                if(c->event_ids[i]==event) c->event_flags[i]=0;
        }else{
            const int on_ship=pharaoh_formation_legion_is_on_ship(
                formation->fields.members,c->walkers)!=0;
            ++c->player_counters[7];
            ++c->player_counters[on_ship?5:6];
            if(formation->fields.flag_67!=0 && formation->fields.member_count!=0){
                ++c->player_counters[8];
                ++c->player_counters[on_ship?0:1];
            }
            c->player_counters[10]=(uint8_t)(c->player_counters[10]+
                formation->fields.member_count);
        }
    }
    for(int32_t i=0;i<removed_count;++i){
        if(c->event_flags[i]==0) continue;
        const int32_t event=c->event_ids[i];
        if(event>=0 && event<*c->event_count)
            ((uint8_t *)&c->events[event])[50]=0;
    }
}
