#include "pharaoh/city/labor.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohLaborCategory)==20 &&
    sizeof(PharaohLaborTypeCategory)==20 &&
    sizeof(PharaohLaborBuildingModel)==40 && sizeof(PharaohLaborWeight)==8,
    "native labor strides");
_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,building_type)==0x10 &&
    offsetof(PharaohBuildingTypeRecord,houses_covered)==0x18 &&
    offsetof(PharaohBuildingTypeRecord,reserved_1a)==0x1a &&
    offsetof(PharaohBuildingTypeRecord,resource_id)==0x43,
    "native labor building fields");

static int32_t signed_dword(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}
static int32_t add_dword(int32_t a,int32_t b)
{ return signed_dword((uint32_t)a+(uint32_t)b); }
static int32_t subtract_dword(int32_t a,int32_t b)
{ return signed_dword((uint32_t)a-(uint32_t)b); }
static uint16_t building_flag_38(const PharaohBuildingTypeRecord *building)
{
    uint16_t value;
    memcpy(&value,(const uint8_t *)building+0x38,sizeof(value));
    return value;
}
static void store_share(PharaohBuildingTypeRecord *building,int32_t value)
{
    const uint16_t bits=(uint16_t)(uint32_t)value;
    memcpy(building->reserved_1a,&bits,sizeof(bits));
}

void pharaoh_city_labor_rebuild_demand(const PharaohCityLaborContext *c)
{
    for(int32_t i=0;i<10;++i){
        c->categories[i].building_count=0;
        c->categories[i].covered=0;
        c->categories[i].allocated=0;
        c->categories[i].demand=0;
    }
    for(uint32_t id=1;signed_dword(id)<=*c->highest_building_id;++id){
        PharaohBuildingTypeRecord *const building=&c->buildings[id];
        if(building->state!=1) continue;
        int32_t group=c->type_categories[building->building_type].category;
        if(group==0 && (uint8_t)pharaoh_building_is_floodplain_farm(
                c->industry,(int32_t)id)!=0) group=-1;
        ((uint8_t *)building)[0x42]=(uint8_t)(uint32_t)group;
        if(group<0) continue;
        if(group==3 && building_flag_38(building)!=0) continue;
        if((group==0 || group==1) && pharaoh_building_resource_is_permitted(
                c->buildings,(int32_t)id,c->resource_flags)!=0) continue;
        const int32_t covered=building->houses_covered;
        if(covered<=0) continue;
        PharaohLaborCategory *const category=&c->categories[group];
        category->demand=add_dword(category->demand,
            c->models[building->building_type].workers);
        category->covered=add_dword(category->covered,covered);
        category->building_count=add_dword(category->building_count,1);
    }
}

PharaohPercentageScaleStatus pharaoh_city_labor_allocate_categories(
    const PharaohCityLaborContext *c)
{
    for(int32_t i=0;i<10;++i) c->categories[i].allocated=0;
    int32_t total=0;
    for(int32_t i=0;i<10;++i) total=add_dword(total,c->categories[i].demand);
    *c->unfilled=0;
    int32_t left=*c->available;
    if(total>left){
        for(int32_t rank=1;rank<=9;++rank){
            for(int32_t i=0;i<9;++i){
                PharaohLaborCategory *const category=&c->categories[i];
                if(category->priority!=rank) continue;
                int32_t take=category->demand;
                if(take>left) take=left;
                category->allocated=take;
                left=subtract_dword(left,take);
                break;
            }
            if(left<=0) break;
        }
        uint32_t round=1;
        if(*c->available>1) while(left>0){
            for(int32_t i=0;i<9;++i){
                const PharaohLaborWeight *const weight=&c->weights[i];
                PharaohLaborCategory *const category=
                    &c->categories[weight->category];
                if(category->priority!=0) continue;
                const int32_t current=category->allocated;
                const int32_t space=subtract_dword(category->demand,current);
                if(space<=0) continue;
                int32_t take=weight->weight;
                if(take>left) take=left;
                if(take>space) take=space;
                left=subtract_dword(left,take);
                category->allocated=add_dword(current,take);
                if(left<=0) break;
            }
            if(left<=0) break;
            ++round;
            if(signed_dword(round)>=*c->available) break;
        }
        *c->employed=*c->available;
        for(int32_t i=0;i<9;++i)
            *c->unfilled=add_dword(*c->unfilled,subtract_dword(
                c->categories[i].demand,c->categories[i].allocated));
    }else{
        for(int32_t i=0;i<10;++i)
            c->categories[i].allocated=c->categories[i].demand;
        *c->employed=total;
    }
    *c->free_workers=subtract_dword(*c->available,*c->employed);
    const PharaohPercentageScaleResult percentage=pharaoh_scale_value_to_percent(
        *c->free_workers,*c->available);
    if(percentage.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)
        return percentage.status;
    *c->unemployment_rate=percentage.value;
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

void pharaoh_city_labor_rebuild_building_shares(const PharaohCityLaborContext *c)
{
    (void)pharaoh_scale_value_to_percent(100,c->categories[8].building_count);
    for(uint32_t id=1;signed_dword(id)<=*c->highest_building_id;++id){
        PharaohBuildingTypeRecord *const building=&c->buildings[id];
        if(building->state!=1) continue;
        const int32_t group=
            c->type_categories[building->building_type].category;
        if(group==0){
            if((uint8_t)pharaoh_building_is_floodplain_farm(c->industry,
                    (int32_t)id)!=0) continue;
        }else if(group==3){
            if(building_flag_38(building)!=0) continue;
        }else if(group<0) continue;
        store_share(building,0);
        const int32_t denominator=c->categories[group].covered;
        const int32_t covered=building->houses_covered;
        if(covered!=0){
            /* Signed WORD*100*100 fits int32; this use cannot raise IDIV
               overflow. Zero denominator retains the native zero result. */
            const PharaohPercentageScaleResult share=
                pharaoh_scale_value_to_percent(covered*100,denominator);
            store_share(building,share.value);
        }
    }
}
