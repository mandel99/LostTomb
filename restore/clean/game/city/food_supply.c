#include "pharaoh/city/food_supply.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/core/percentage_adjust.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,house_size)==5 &&
    offsetof(PharaohBuildingTypeRecord,house_population)==28 &&
    offsetof(PharaohBuildingTypeRecord,num_workers)==64 &&
    offsetof(PharaohBuildingTypeRecord,has_road_access)==68 &&
    offsetof(PharaohBuildingTypeRecord,house_food_54)==84 &&
    offsetof(PharaohBuildingTypeRecord,granary_food_56)==86 &&
    sizeof(PharaohBuildingModelRecord)==40 &&
    offsetof(PharaohBuildingModelRecord,laborers)==20,
    "native food storage overlays");

static int32_t signed_total(uint32_t value)
{
    return value<=INT32_MAX?(int32_t)value:
        (int32_t)((int64_t)value-INT64_C(0x100000000));
}

void pharaoh_city_rebuild_food(const PharaohCityFoodContext *c)
{
    uint32_t house_food[9]={0},stored_62=0,stored_66=0;
    for(int32_t i=0;i<9;++i) c->stocks[i]=0;
    const int32_t initial_count=*c->building_count;
    *c->total_stored=0;*c->available_types=0;
    *c->months=0;*c->staffed_granaries=0;
    *c->under_100=0;*c->under_50=0;*c->under_50_with_food=0;
    if(initial_count>=1) for(int32_t id=1;;++id){
        PharaohBuildingTypeRecord *const b=&c->buildings[id];
        if(b->state==1){
            if(b->building_type==71 || b->building_type==191){
                b->has_road_access=0;
                if(pharaoh_map_has_road_access_granary(c->road,b->x,b->y)){
                    b->has_road_access=1;
                    /* Signed WORD workers*100 is in[-3276800,3276700];
                       the native IDIV overflow case is impossible here. */
                    const int32_t coverage=pharaoh_scale_value_to_percent(
                        b->num_workers,c->models[b->building_type].laborers).value;
                    if(coverage<100) ++*c->under_100;
                    if(coverage<50){
                        uint32_t total=0;
                        for(int32_t i=1;i<=8;++i)
                            total+=(uint32_t)(int32_t)b->granary_food_56[i];
                        ++*c->under_50;
                        if(signed_total(total)>0) ++*c->under_50_with_food;
                    }else{
                        stored_62+=(uint32_t)(int32_t)b->granary_food_56[6];
                        stored_66+=(uint32_t)(int32_t)b->granary_food_56[8];
                        for(int32_t i=0;i<9;++i)
                            c->stocks[i]+=(uint32_t)(int32_t)b->granary_food_56[i];
                        ++*c->staffed_granaries;
                        const PharaohTutorialCompletionContext *const t=c->tutorial;
                        if(*t->advisor_override==0){
                            if(*t->mission_id==t->rules[2].mission_threshold &&
                                t->flags[2]==0 && t->flags[1]!=0 &&
                                signed_total(stored_66)>=400){
                                pharaoh_tutorial_set_flag(t,2);
                                if(*t->advisor_override!=0) goto next_food;
                            }
                            if(*t->mission_id==t->rules[7].mission_threshold &&
                                t->flags[7]==0 && signed_total(stored_62)>=800)
                                pharaoh_tutorial_set_flag(t,7);
                        }
                    }
                }
            }else if(b->house_size!=0 && b->house_population!=0){
                for(int32_t i=0;i<8;++i)
                    house_food[i+1]+=(uint32_t)(int32_t)b->house_food_54[i];
            }
        }
next_food:
        if(id+1>*c->building_count) break;
    }
    for(int32_t i=1;i<=8;++i) if(c->stocks[i]!=0 || house_food[i]!=0){
        *c->total_stored+=c->stocks[i];
        ++*c->available_types;
    }
    const int32_t rate=pharaoh_city_food_consumption_rate(c->rate);
    const int32_t need=pharaoh_adjust_by_percentage(*c->population,rate);
    *c->monthly_need=need;
    if(need<=0) *c->months=signed_total(*c->total_stored)>0;
    else *c->months=(uint32_t)(signed_total(*c->total_stored)/need);
    if(*c->editor_active!=0){*c->available_types=1;*c->months=12;}
}

void pharaoh_city_update_food(const PharaohCityFoodUpdateContext *c)
{
    if(*c->enabled!=0) pharaoh_city_rebuild_food(c->player_one);
    const int32_t count=*c->building_count;
    if(count<1) return;
    const uint32_t editor=*c->editor_active;
    for(int32_t id=1;id<=count;++id){
        PharaohBuildingTypeRecord *const b=&c->buildings[id];
        if(b->state==1 && (b->building_type==70 || b->building_type==190) &&
            editor!=0) b->granary_food_56[0]=200;
    }
}
