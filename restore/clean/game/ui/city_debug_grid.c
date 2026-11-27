#include "pharaoh/ui/city_debug_grid.h"
#include "pharaoh/map/service_visit.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264,"Native building stride");
_Static_assert(offsetof(PharaohBuildingTypeRecord,farm_water_access)==0x35,"water BYTE");
_Static_assert(offsetof(PharaohBuildingTypeRecord,houses_covered)==0x18,"coverage WORD");
_Static_assert(offsetof(PharaohBuildingTypeRecord,distance_from_entry)==0x20,"distance WORD");
_Static_assert(offsetof(PharaohBuildingTypeRecord,house_population)==0x1c,"population WORD");
_Static_assert(offsetof(PharaohBuildingTypeRecord,reserved_44)==0x44,"damage overlay WORD+46");

static int32_t signed_bits(uint32_t value)
{
    int32_t result;
    memcpy(&result,&value,sizeof(result));
    return result;
}

void pharaoh_city_debug_draw_grid_value(
    const PharaohCityDebugGridDrawContext *c,int32_t x,int32_t y)
{
    const PharaohCityDebugGridValues *const g=c->grids;
    const uint32_t tile=*c->tile;
    const int32_t building=g->building_ids[tile];
    uint32_t value=(uint32_t)building;
    uint16_t color=UINT16_C(0xffff);
    switch(*c->mode) {
    case 0:break;
    case 1:value=g->terrain[tile];break;
    case 2:value=g->bitfields[tile];break;
    case 3:value=g->images[tile];break;
    case 4:value=g->edges[tile];break;
    case 5:value=g->canals[tile];break;
    case 6:value=g->random[tile];break;
    case 7:value=(uint32_t)g->walker_ids[tile];break;
    case 8:value=g->sprites[tile];break;
    case 9:value=(uint32_t)g->land_citizen[tile];break;
    case 10:value=g->routing_distance[tile];break;
    case 11:value=c->buildings[building].farm_water_access;break;
    case 12:value=(uint32_t)c->buildings[building].houses_covered;break;
    case 13:value=(uint32_t)c->buildings[building].distance_from_entry;break;
    case 14: {
        int16_t damage_risk;
        /* Actual WORD+46 overlaps the type-specific service bytes in the
         * shared264-byte record. Do not reinterpret that word as a BYTE. */
        memcpy(&damage_risk,c->buildings[building].reserved_44+2,sizeof(damage_risk));
        value=(uint32_t)damage_risk;
        break;
    }
    case 15:value=(uint32_t)c->buildings[building].house_population;break;
    case 16:value=(uint32_t)g->desirability[tile];break;
    case 17:value=g->elevation[tile];break;
    case 18:value=(uint32_t)g->water[tile];break;
    case 19:value=(uint32_t)g->land_noncitizen[tile];break;
    case 20:value=(uint32_t)g->building_damage[tile];break;
    case 21:value=(uint32_t)pharaoh_walker_chain_count(c->walkers,g->walker_ids,signed_bits(tile));break;
    case 22:value=g->old_types[tile];break;
    case 23:value=g->soldier_strength[tile];break;
    case 24:value=(uint32_t)g->walls[tile];break;
    case 25:value=g->road_network[tile];break;
    case 26:value=g->road_segment[tile];break;
    case 27:value=(uint32_t)pharaoh_map_service_visit_get(g->service_visits,1,signed_bits(tile));break;
    default:return;
    }
    if(value==0)return;
    if((value&UINT32_C(0x80000000))!=0) {
        value=0u-value;
        color=UINT16_C(0xf800);
    }
    pharaoh_bitmap_text_draw_number_native(c->font,c->text,c->number_text,
        signed_bits(value),' ',c->suffix,signed_bits((uint32_t)x+24u),
        signed_bits((uint32_t)y+10u),*c->font_id,color);
}

int32_t pharaoh_city_debug_grid_bound_for_current_mode(
    const PharaohCityDebugGridDrawContext *c)
{
    if(!c||!c->tile||!c->mode||!c->grids||!c->grids->building_ids)return 0;
    const PharaohCityDebugGridValues *const g=c->grids;
    const int32_t mode=*c->mode;
    if(mode<0||mode>27)return 1;
    if(!c->font||!c->text||!c->number_text||!c->font_id||!c->suffix)return 0;
    switch(mode) {
    case 0:return 1;
    case 1:return g->terrain!=NULL;
    case 2:return g->bitfields!=NULL;
    case 3:return g->images!=NULL;
    case 4:return g->edges!=NULL;
    case 5:return g->canals!=NULL;
    case 6:return g->random!=NULL;
    case 7:return g->walker_ids!=NULL;
    case 8:return g->sprites!=NULL;
    case 9:return g->land_citizen!=NULL;
    case 10:return g->routing_distance!=NULL;
    case 11:case 12:case 13:case 14:case 15:return c->buildings!=NULL;
    case 16:return g->desirability!=NULL;
    case 17:return g->elevation!=NULL;
    case 18:return g->water!=NULL;
    case 19:return g->land_noncitizen!=NULL;
    case 20:return g->building_damage!=NULL;
    case 21:return g->walker_ids&&c->walkers;
    case 22:return g->old_types!=NULL;
    case 23:return g->soldier_strength!=NULL;
    case 24:return g->walls!=NULL;
    case 25:return g->road_network!=NULL;
    case 26:return g->road_segment!=NULL;
    case 27:return g->service_visits!=NULL;
    default:return 0;
    }
}
