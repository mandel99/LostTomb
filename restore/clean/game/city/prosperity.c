#include "pharaoh/city/prosperity.h"

#include <stddef.h>
#include <string.h>

_Static_assert(PHARAOH_BUILDING_RECORD_COUNT==4000 &&
    sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,grid_offset)==0xc &&
    offsetof(PharaohBuildingTypeRecord,building_type)==0x10 &&
    offsetof(PharaohPyramidTilePlacement,missing_tile)==12,
    "native completed-monument fields");

static int32_t signed_dword(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}
static int32_t add_dword(int32_t a,int32_t b)
{ return signed_dword((uint32_t)a+(uint32_t)b); }
static int32_t subtract_dword(int32_t a,int32_t b)
{ return signed_dword((uint32_t)a-(uint32_t)b); }

int32_t pharaoh_city_temple_complex_wage_bonus(
    const PharaohTempleWageBonusContext *c)
{
    if(*c->temple_count==0) return 0;
    if(pharaoh_building_group_index(c->buildings,*c->temple_building)!=1)
        return 0;
    return pharaoh_temple_upgrade_present(c->buildings,
        *c->temple_building,2)?-2:0;
}

uint8_t pharaoh_city_has_completed_monument(
    const PharaohCompletedMonumentContext *c)
{
    PharaohMapSurfaceContext *const surface=c->surface;
    for(int32_t id=1;id<PHARAOH_BUILDING_RECORD_COUNT;++id){
        const PharaohBuildingTypeRecord *const building=&surface->buildings[id];
        if(building->state!=1) continue;
        if(building->building_type==0xb7){
            int16_t remaining;
            memcpy(&remaining,(const uint8_t *)building+0xc8,sizeof(remaining));
            if(remaining>=1) continue;
            const uint8_t variant=((const uint8_t *)building)[0x54];
            const int32_t relative=signed_dword(
                (uint32_t)c->pyramid_anchor_offsets[variant]+
                (uint32_t)building->grid_offset-(uint32_t)surface->map->origin);
            pharaoh_pyramid_map_segment_update(surface,
                relative%228,relative/228,3,0);
            if(surface->pyramid_placement->missing_tile==0) return 1;
        }else if(building->building_type==0xd2 && *c->sphinx_stage>=16)
            return 1;
    }
    return 0;
}

PharaohPercentageScaleStatus pharaoh_city_ratings_prosperity_explanation(
    const PharaohProsperityExplanationContext *c)
{
    const int32_t bonus=pharaoh_city_temple_complex_wage_bonus(c->temple);
    int32_t income=*c->income[0],expense=*c->expense[0];
    for(int32_t i=1;i<4;++i) income=add_dword(income,*c->income[i]);
    for(int32_t i=1;i<5;++i) expense=add_dword(expense,*c->expense[i]);
    const int32_t net=subtract_dword(income,expense);
    const int32_t extra=*c->value_ea44fc;
    int32_t score=0,priority=0,reason=4;
    if(net>0) score=5;
    else if(add_dword(subtract_dword(extra/2,expense),income)>0) score=3;
    else if(net<0){priority=8;score=-8;}
    else if(add_dword(subtract_dword(extra,expense),income)<0){
        priority=5;score=-5;
    }
    const int32_t value=*c->value_ea41e4;
    if(value>=0 && value<=1) --score;
    const int32_t first=*c->income[3],second=*c->expense[4];
    if(first>second) ++score;
    else if(first<second){
        if(priority<1){priority=1;reason=11;}
        --score;
    }else if(priority==0){priority=1;reason=11;}

    if(*c->unfilled>0){
        if(priority<1){priority=1;reason=10;}
        --score;
    }else{
        const int32_t unemployment=*c->unemployment;
        if(unemployment<5) score+=2;
        else if(unemployment<10){
            if(priority==0){priority=1;reason=5;}
        }else if(unemployment<15){
            --score;
            if(priority<1){priority=1;reason=5;}
        }else if(unemployment<20){
            score-=2;
            if(priority<2){priority=2;reason=5;}
        }else{
            score-=3;
            if(priority<3){priority=3;reason=5;}
        }
    }
    const int32_t gap=subtract_dword(subtract_dword(
        *c->value_ea5e18/12,*c->value_ea447c),bonus);
    if(gap>5) score+=2;
    else if(gap>0) ++score;
    else if(gap<-10){
        if(priority<3){priority=3;reason=6;}
        score-=3;
    }else if(gap<-5){
        score-=2;
        if(priority<2){priority=2;reason=6;}
    }else if(gap<0){
        --score;
        if(priority<1){priority=1;reason=6;}
    }
    const PharaohPercentageScaleResult low=pharaoh_scale_value_to_percent(
        *c->value_ea3f78,*c->population);
    if(low.status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return low.status;
    const PharaohPercentageScaleResult high=pharaoh_scale_value_to_percent(
        *c->value_ea3f7c,*c->population);
    if(high.status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return high.status;
    if(low.value>95){
        if(priority<1){priority=1;reason=7;}
        --score;
    }
    if(high.value>10) score+=2;
    if(*c->value_ea5d58!=0){
        --score;
        if(priority<1){priority=1;reason=8;}
    }
    if(*c->value_ea5be8>0) ++score;
    if(pharaoh_city_has_completed_monument(c->monuments)!=0) ++score;

    const int32_t current=*c->prosperity;
    const int32_t next=add_dword(current,score);
    if(score==0 || (current<=0 && *c->current_year==*c->scenario_start_year)){
        *c->explanation=0;
        *c->trend=2;
    }else if(score<=0){
        *c->trend=1;
        *c->explanation=reason;
    }else{
        *c->trend=0;
        const int32_t cap=*c->cap;
        if(next<=cap) *c->explanation=priority>0?reason:0;
        else{
            const int32_t reloaded=*c->prosperity;
            if(reloaded>cap) *c->trend=1;
            else *c->trend=reloaded>=cap?2:0;
            *c->explanation=3;
        }
    }
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}
