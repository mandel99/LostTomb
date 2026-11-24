#include "pharaoh/rendering/minimap_metric_bindings.h"
#include "pharaoh/core/percentage_scale.h"

#include <string.h>

static int32_t dword_at(const PharaohBuildingTypeRecord *b, size_t offset)
{
    int32_t value;
    memcpy(&value, (const uint8_t *)b + offset, sizeof(value));
    return value;
}

static int32_t enabled(const PharaohMinimapMetricBindings *c,int mode,int slot)
{ return *c->service_enabled[mode][slot]; }

static int32_t religion(void *data,int32_t id)
{
    PharaohMinimapMetricBindings *c=data;const PharaohBuildingTypeRecord *b=&c->native->buildings[id];
    switch(b->building_type) {case 0x62:case 0x96:case 0x3c:case 0x3d:case 0x3e:
    case 0x3f:case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0xd2:return 0;}
    if(b->house_size==0) return 0;
    int32_t total=*c->oracle_total;for(int i=0;i<15;++i) total+=c->temple_counts[i];
    return total==0?0:((int8_t)*((const uint8_t *)b+0x83)*17)/10;
}
static int32_t fire_risk(void *d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_minimap_fire_risk_height(c->native,id);}
static int32_t damage_risk(void *d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_minimap_damage_risk_height(c->native,id);}
static int32_t crime(void *data,int32_t id)
{PharaohMinimapMetricBindings*c=data;const PharaohBuildingTypeRecord*b=&c->native->buildings[id];return (b->house_size==0||b->building_type==0x37||b->building_type==0xb8||b->building_type==99)?0:dword_at(b,0xe4)/100;}
static int32_t entertainment(void *data,int32_t id)
{PharaohMinimapMetricBindings*c=data;const PharaohBuildingTypeRecord*b=&c->native->buildings[id];*c->native->active_sprite_image=c->native->map_image[*c->native->current_map_cell];if((b->building_type>=30&&b->building_type<=37)||b->building_type==226||b->house_size==0)return 0;int32_t total=0;for(int i=0;i<9;++i)total+=c->venue_counts[i];return total==0?0:10-(int8_t)*((const uint8_t*)b+0x80)/10;}
static int32_t service12(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_building_service_need_004340e0(c->native->buildings,id,enabled(c,12,0),enabled(c,12,1));}
static int32_t service13(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_building_service_need_00434150(c->native->buildings,id,enabled(c,13,0),enabled(c,13,1),enabled(c,13,2));}
static int32_t service14(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_building_service_need_004341d0(c->native->buildings,id,enabled(c,14,0),enabled(c,14,1),enabled(c,14,2));}
static int32_t service15(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_building_service_need_00434250(c->native->buildings,id,enabled(c,15,0),enabled(c,15,1));}
static int32_t service16(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_building_service_need_00434250(c->native->buildings,id,enabled(c,16,0),enabled(c,16,1));}
static int32_t service17(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_building_service_status_00434060(c->native->buildings,id,enabled(c,17,0),enabled(c,17,1));}
#define ONE(name,mode,fn) static int32_t name(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return fn(c->native->buildings,id,enabled(c,mode,0));}
ONE(service18,18,pharaoh_building_service_need_00434510)
ONE(service19,19,pharaoh_building_service_need_00434570)
ONE(service21,21,pharaoh_building_service_need_004345d0)
ONE(service22,22,pharaoh_building_service_need_00434630)
ONE(service23,23,pharaoh_building_service_need_00434690)
ONE(service24,24,pharaoh_building_service_need_004346f0)
ONE(service45,45,pharaoh_building_service_need_004343a0)
ONE(service50,50,pharaoh_building_service_need_00434750)
static int32_t tax(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_minimap_tax_income_height(c->native,id);}
static int32_t food(void*data,int32_t id)
{PharaohMinimapMetricBindings*c=data;const PharaohBuildingTypeRecord*b=&c->native->buildings[id];PharaohOverlayFoodBuilding f={0};f.type=b->building_type;f.house_size=b->house_size;f.house_level=b->subtype;f.population=b->house_population;memcpy(f.stored_food,(const uint8_t*)b+0x54,sizeof(f.stored_food));return pharaoh_overlay_food_stock_height(&f,c->house_model_food_type_counts,*c->food_supply_available);}
static int32_t fertility(void*d,int32_t tile){PharaohMinimapMetricBindings*c=d;return pharaoh_overlay_fertility_height(c->tile_metrics,tile);}
static int32_t desirability(void*d,int32_t tile){PharaohMinimapMetricBindings*c=d;return pharaoh_overlay_desirability_height(c->tile_metrics,tile);}
static int32_t labor(void*data,int32_t id){PharaohMinimapMetricBindings*c=data;const PharaohBuildingTypeRecord*b=&c->native->buildings[id];if(b->house_size!=0)return 0;return pharaoh_scale_value_to_percent(b->houses_covered,300).value/10;}
static int32_t one(void*d,int32_t id){(void)d;(void)id;return 1;}
static int32_t missing(void*d,int32_t id){PharaohMinimapMetricBindings*c=d;return pharaoh_minimap_storage_delivery_missing(c->native,id);}
static int32_t malaria(void*data,int32_t id){PharaohMinimapMetricBindings*c=data;const PharaohBuildingTypeRecord*b=&c->native->buildings[id];return b->house_size==0?0:dword_at(b,0xe8)/100;}
static int32_t disease(void*data,int32_t id){PharaohMinimapMetricBindings*c=data;const PharaohBuildingTypeRecord*b=&c->native->buildings[id];return b->house_size==0?0:dword_at(b,0xfc)/100;}
static int32_t listed(void*data,int32_t id){PharaohMinimapMetricBindings*c=data;return pharaoh_building_type_in_list(c->native->buildings,id,c->overlay_type_list);}
static int32_t terrain_damage(void*d,int32_t tile){PharaohMinimapMetricBindings*c=d;return pharaoh_overlay_damage_terrain_height(c->damage_terrain,tile);}
static int32_t resource(void*d,int32_t id,int32_t kind,int32_t slot){PharaohMinimapMetricBindings*c=d;return pharaoh_minimap_resource_height(c->native,id,kind,slot);}

void pharaoh_minimap_bind_native_metrics(PharaohMinimapBuilding *b,PharaohMinimapMetricBindings*c)
{
    b->metric_user_data=c;b->height[4]=religion;b->height[8]=fire_risk;b->height[9]=damage_risk;
    b->height[10]=crime;b->height[11]=entertainment;b->height[12]=service12;b->height[13]=service13;
    b->height[14]=service14;b->height[15]=service15;b->height[16]=service16;b->height[17]=service17;
    b->height[18]=service18;b->height[19]=service19;b->height[21]=service21;b->height[22]=service22;
    b->height[23]=service23;b->height[24]=service24;b->height[25]=tax;b->height[26]=food;
    b->height[27]=fertility;b->height[28]=desirability;b->height[29]=labor;b->height[30]=one;
    b->height[31]=missing;b->height[43]=malaria;b->height[45]=service45;b->height[48]=disease;
    b->height[49]=listed;b->height[50]=service50;b->terrain_damage=terrain_damage;b->resource_height=resource;
}
