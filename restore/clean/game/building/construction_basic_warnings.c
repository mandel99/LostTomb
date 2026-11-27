#include "pharaoh/building/construction_basic_warnings.h"
#include <string.h>

_Static_assert(sizeof(PharaohBuildingModelRecord)==40 &&
    offsetof(PharaohBuildingModelRecord,laborers)==20 &&
    sizeof(PharaohScenarioPlayerRuntime)==0x49d8,"native model/player strides");

static int32_t player_i32(const PharaohConstructionBasicWarnings *c,uint32_t offset)
{
    int32_t value;
    const uint8_t *player=(const uint8_t *)&c->players[*c->current_player];
    memcpy(&value,player+offset,sizeof(value));
    return value;
}
static void missing_building(const PharaohConstructionBasicWarnings *c,
    int32_t tool,int32_t expected,const int32_t *count,int32_t message)
{
    if(*c->editor_active==1||*c->warnings->redraw)return;
    if(tool==expected&&*count<=0)pharaoh_city_warning_post(c->warnings,message);
}

/* Complete MapCommitWarnings C with actual live pointers and real post child. */
void pharaoh_construction_warning_workers(const PharaohConstructionBasicWarnings *c,int32_t tool)
{
    if(*c->editor_active==1)return;
    if(tool==92||*c->warnings->redraw)return;
    if(c->models[tool].laborers<=0)return;
    if(player_i32(c,0x2c04)<10)return;
    pharaoh_city_warning_post(c->warnings,17);
}
void pharaoh_construction_warning_granary(const PharaohConstructionBasicWarnings *c,int32_t tool)
{missing_building(c,tool,71,c->granary_count,19);}
void pharaoh_construction_warning_booth(const PharaohConstructionBasicWarnings *c,int32_t tool)
{missing_building(c,tool,31,c->booth_count,44);}
void pharaoh_construction_warning_bandstand(const PharaohConstructionBasicWarnings *c,int32_t tool)
{missing_building(c,tool,30,c->bandstand_count,45);}
void pharaoh_construction_warning_pavilion(const PharaohConstructionBasicWarnings *c,int32_t tool)
{missing_building(c,tool,33,c->pavilion_count,46);}
void pharaoh_construction_warning_fort(const PharaohConstructionBasicWarnings *c,int32_t tool)
{
    if(*c->editor_active==1||*c->warnings->redraw)return;
    if((tool==40||tool==44||tool==45)&&*c->fort_count<=0)
        pharaoh_city_warning_post(c->warnings,42);
}
void pharaoh_construction_warning_recruiter(const PharaohConstructionBasicWarnings *c,int32_t tool)
{
    if(*c->editor_active==1||*c->warnings->redraw)return;
    if(tool!=95)return;
    /* Native signed weapons stock, player48F0+resource10*4. */
    if(player_i32(c,0x4918)<=0)pharaoh_city_warning_post(c->warnings,43);
}
void pharaoh_construction_warning_well(const PharaohConstructionBasicWarnings *c,
    int32_t tool,int32_t x,int32_t y)
{
    if(*c->editor_active==1||*c->warnings->redraw)return;
    if(tool!=92)return;
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x;
    if(!(c->map->terrain[tile]&0x80u))pharaoh_city_warning_post(c->warnings,38);
}

/* Reuse actual complete Rule bodies from PlacementPreviewPredicates.
 * Full native EAX contains masked terrain; only the logical AL is exposed. */
uint8_t pharaoh_construction_terrain_rule_00405b40(uint32_t terrain)
{return (terrain&UINT32_C(0x20300002))==UINT32_C(0x00100002);}
uint8_t pharaoh_construction_terrain_rule_00405b60(uint32_t terrain)
{return (terrain&UINT32_C(0x20300002))==2;}
uint8_t pharaoh_construction_terrain_rule_00405b80(uint32_t terrain)
{return (terrain&2)==2;}

PharaohPercentageScaleStatus pharaoh_construction_warning_food_supply(
    const PharaohConstructionBasicWarnings *c,int32_t tool)
{
    if(*c->editor_active==1||*c->warnings->redraw||tool!=10)
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    if(c->players[*c->current_player].population<300)
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    if(*c->native_mode_784bbc!=0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    /* Native pushes this DWORD before447530; retain the cached denominator. */
    const int32_t maximum=player_i32(c,0x45dc);
    const PharaohFoodSupplyQueryResult supply=pharaoh_city_food_supply_query(c->food_supply);
    if(supply.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return supply.status;
    const PharaohPercentageScaleResult percent=pharaoh_scale_value_to_percent(supply.value,maximum);
    if(percent.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return percent.status;
    if(percent.value<=95)pharaoh_city_warning_post(c->warnings,94);
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}
