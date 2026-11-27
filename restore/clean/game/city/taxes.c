#include "pharaoh/city/taxes.h"
#include "pharaoh/core/percentage_adjust.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohHouseTaxMetric)==88 &&
    sizeof(PharaohTaxWeightedBand)==8,"native tax field strides");
_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,house_size)==5 &&
    offsetof(PharaohBuildingTypeRecord,subtype)==0x12 &&
    offsetof(PharaohBuildingTypeRecord,house_population)==0x1c &&
    offsetof(PharaohBuildingTypeRecord,tax_collector_service)==0x4e &&
    offsetof(PharaohBuildingTypeRecord,house_tax_income)==0x8c,
    "native house tax fields");

static int32_t signed_dword(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}
static uint32_t adjust_tax(const PharaohCityTaxContext *c,uint32_t weighted)
{
    const int32_t taxed=pharaoh_adjust_by_percentage(
        signed_dword(weighted),*c->tax_rate);
    return (uint32_t)pharaoh_adjust_by_percentage(taxed,
        c->difficulty_scales[*c->difficulty]);
}
static int32_t bonus_present(const PharaohBuildingTypeRecord *house)
{ return ((const uint8_t *)house)[0x88]!=0; }
static void add_history(PharaohBuildingTypeRecord *house,uint32_t value)
{
    uint32_t history;
    memcpy(&history,(const uint8_t *)house+0x8c,sizeof(history));
    history+=value;
    memcpy((uint8_t *)house+0x8c,&history,sizeof(history));
}

PharaohPercentageScaleStatus pharaoh_city_taxes_collect_month(
    const PharaohCityTaxContext *c)
{
    c->population_collected[0]=0;
    c->population_collected[1]=0;
    c->population_uncollected[0]=0;
    c->population_uncollected[1]=0;
    for(int32_t i=0;i<2;++i){
        c->weighted[i].uncollected=0;
        c->weighted[i].collected=0;
    }
    for(int32_t i=0;i<20;++i) c->tier_population[i]=0;
    for(uint32_t id=1;signed_dword(id)<=*c->highest_building_id;++id){
        PharaohBuildingTypeRecord *const house=&c->buildings[id];
        if(house->state!=1 || house->house_size==0) continue;
        const int32_t tier=house->subtype;
        const int32_t band=tier>=14;
        uint32_t metric=c->metrics[tier].metric;
        if(bonus_present(house)) metric=(uint32_t)pharaoh_adjust_by_percentage(
            signed_dword(metric),110);
        const int32_t reloaded_tier=house->subtype;
        const uint32_t population=(uint32_t)(int32_t)house->house_population;
        c->tier_population[reloaded_tier]+=population;
        const uint32_t weighted=population*metric;
        if(house->tax_collector_service!=0){
            c->population_collected[band]+=population;
            const uint32_t history=
                (uint32_t)c->difficulty_scales[*c->difficulty]*
                (uint32_t)*c->tax_rate*weighted;
            add_history(house,history);
            c->weighted[band].collected+=weighted;
        }else{
            c->population_uncollected[band]+=population;
            c->weighted[band].uncollected+=weighted;
        }
    }
    uint32_t income=adjust_tax(c,c->weighted[1].collected);
    c->year_collected[1]+=income;
    const uint32_t plebs=adjust_tax(c,c->weighted[0].collected);
    income+=plebs;
    c->year_collected[0]+=plebs;
    c->year_uncollected[1]+=adjust_tax(c,c->weighted[1].uncollected);
    c->year_uncollected[0]+=adjust_tax(c,c->weighted[0].uncollected);
    *c->treasury+=income;

    uint32_t numerator=c->population_collected[0];
    uint32_t denominator=c->population_uncollected[0]+numerator;
    PharaohPercentageScaleResult rate=pharaoh_scale_value_to_percent(
        signed_dword(numerator),signed_dword(denominator));
    if(rate.status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return rate.status;
    c->coverage[0]=(uint32_t)rate.value;
    numerator=c->population_collected[1];
    denominator=c->population_uncollected[1]+numerator;
    rate=pharaoh_scale_value_to_percent(
        signed_dword(numerator),signed_dword(denominator));
    if(rate.status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return rate.status;
    c->coverage[1]=(uint32_t)rate.value;
    denominator=c->population_uncollected[1]+c->population_uncollected[0];
    numerator=c->population_collected[0];
    const uint32_t nobles=c->population_collected[1];
    denominator+=numerator;
    numerator+=nobles;
    denominator+=nobles;
    rate=pharaoh_scale_value_to_percent(
        signed_dword(numerator),signed_dword(denominator));
    if(rate.status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return rate.status;
    c->coverage[2]=(uint32_t)rate.value;
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

void pharaoh_city_taxes_estimate(const PharaohCityTaxContext *c)
{
    const int32_t limit=*c->highest_building_id;
    c->weighted[0].collected=0;
    c->weighted[1].collected=0;
    if(limit>=1) for(int32_t index=0;index<limit;++index){
        const PharaohBuildingTypeRecord *const house=&c->buildings[index+1];
        if(house->state!=1 || house->house_size==0) continue;
        const int32_t tier=house->subtype;
        const uint32_t metric=c->metrics[tier].metric;
        if(house->tax_collector_service!=0)
            c->weighted[tier>=14].collected+=
                (uint32_t)(int32_t)house->house_population*metric;
    }
    const uint32_t nobles=adjust_tax(c,c->weighted[1].collected);
    const uint32_t plebs=adjust_tax(c,c->weighted[0].collected);
    const uint32_t collected=c->year_collected[0]+c->year_collected[1];
    const uint32_t remaining_months=UINT32_C(12)-*c->month;
    *c->taxes_this_year=collected;
    *c->forecast=(nobles+plebs)*remaining_months+collected;
}
