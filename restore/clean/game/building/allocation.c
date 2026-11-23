#include "pharaoh/building/allocation.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/scenario/scene_industry.h"
#include "pharaoh/simulation/industry_queries.h"
#include <stddef.h>
_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 && offsetof(PharaohBuildingTypeRecord,allocation_native_56)==0x56 && offsetof(PharaohBuildingTypeRecord,native_c2)==0xc2 && offsetof(PharaohBuildingTypeRecord,native_e4)==0xe4 && offsetof(PharaohBuildingTypeRecord,house_malaria_risk)==0xe8 && offsetof(PharaohBuildingTypeRecord,health_distance_4)==0xf4 && offsetof(PharaohBuildingTypeRecord,health_distance_40000)==0xf8,"native allocation/health fields");
_Static_assert(offsetof(PharaohScenarioPlayerRuntime,allocation_reserved_id)==0x15 && offsetof(PharaohScenarioPlayerRuntime,health_rating)==0x28,"native player fields");
static int32_t undo_contains(PharaohBuildingAllocationContext *c,int32_t id)
{return pharaoh_native_undo_contains_004851b0(id,*c->undo.ready,*c->undo.mode,*c->undo.count,c->undo.ids);}
static bool farm(void *data,int32_t id)
{PharaohBuildingAllocationContext *c=data;return (uint8_t)pharaoh_building_is_farm(c->surface->buildings,id)!=0;}
static bool occupied(void *data,int32_t id)
{
 PharaohBuildingAllocationContext *c=data;
 const PharaohBuildingTypeRecord *b=&c->surface->buildings[id];
 /* Inline native894A0's coordinate adaptation; retain native AL consumption. */
 c->occupancy->grid_origin=(uint32_t)c->surface->map->origin;
 const uint8_t result=(uint8_t)pharaoh_region_has_active_occupant(c->occupancy,b->x,b->y,3,3);
 *c->surface->current_tile=c->occupancy->last_origin;
 return result!=0;
}
static int32_t floodplain_farm(PharaohBuildingAllocationContext *c,int32_t id)
{const PharaohIndustryQueryContext q={.is_farm=farm,.is_on_floodplain=occupied,.user_data=c};return pharaoh_building_is_floodplain_farm(&q,id);}
/* Completed prior BuildingAllocator C, native0046A400; slots are not memset. */
int32_t pharaoh_building_allocate(PharaohBuildingAllocationContext *c,uint32_t owner,uint32_t kind,int32_t x,int32_t y,int32_t contiguous,int32_t selected)
{
 uint32_t id=(uint32_t)selected,i,limit,start,serial,cell,value,Angle,quarter;
 int16_t type;
 if(!id){
  id=1;limit=(uint32_t)contiguous+1u;
  for(;;){
   if(!c->surface->buildings[id].state&&!undo_contains(c,(int32_t)id)){
    if(contiguous<=1)break;
    start=limit+(1u-(uint32_t)contiguous);
    if((int32_t)start>=(int32_t)limit)break;
    for(i=start;(int32_t)i<(int32_t)limit;++i)if(c->surface->buildings[i].state)goto next;
    for(i=start;(int32_t)i<(int32_t)limit;++i)if(undo_contains(c,(int32_t)i))goto next;
    break;
   }
next:
   ++id;++limit;if(id>=4000u)goto failure;
  }
 }else if(c->surface->buildings[id].state||undo_contains(c,(int32_t)id))goto failure;
 PharaohBuildingTypeRecord *const b=&c->surface->buildings[id];
 b->state=3;b->player=(uint8_t)owner;b->reserved_id=(c->players[owner].allocation_reserved_id);
 b->building_type=(uint16_t)kind;b->size=(c->definitions[kind].size);
 serial=(*c->next_serial);b->runtime_value_016=(uint16_t)serial;b->native_c2=50;b->distance_from_entry=0;(*c->next_serial)=serial+1u;
 b->house_size=0;type=b->building_type;
 if(type>=10&&type<=19)b->house_size=1;
 else if(type>=20&&type<=23)b->house_size=2;
 else if(type>=24&&type<=27)b->house_size=3;
 else if(type>=28&&type<=29)b->house_size=4;
 type=b->building_type;b->subtype=(type>=10&&type<=29)?(uint16_t)(kind-10u):0;
 b->secondary_resource_model=0;
 if(b->house_size){
  b->native_e4=(c->house_models[b->subtype].crime_base);
  b->house_malaria_risk=0;b->house_plague_risk=0;pharaoh_house_update_health(c->health,(int32_t)id,1);
 }else b->native_e4=0;
 type=b->building_type;
 if((type>=140&&type<=149)||type==202)b->reserved_aa[0]=(uint8_t)(*c->rotation);
 else if(type>=41&&type<=43){
  value=(*c->rotation);Angle=(uint32_t)((int32_t)value%4);quarter=(uint32_t)((int32_t)(value+4u)/4);
  if((int32_t)quarter>=4)quarter=0;
  value=(uint32_t)((int32_t)(*c->surface->orientation)/2)+quarter+1u;if((int32_t)value>=4)value-=4u;
  b->reserved_aa[0]=(uint8_t)((uint8_t)(Angle<<4)+(uint8_t)value);
 }
 type=b->building_type;
 /* This switch preserves every branch's ordered BYTE/WORD writes. */
 switch(type){
 case 0x66:b->resource_id=1;break;case 0x67:b->resource_id=3;break;case 0x68:b->resource_id=5;break;
 case 0x65:b->resource_id=16;break;case 0xe0:b->resource_id=32;break;case 0x69:b->resource_id=4;break;case 0x64:b->resource_id=14;break;
 case 0xc2:b->resource_id=2;b->subtype=13;break;case 0xc4:b->resource_id=6;break;case 0xc3:b->resource_id=22;break;
 case 0x6a:b->resource_id=24;break;case 0xdd:b->resource_id=30;break;case 0x6b:b->resource_id=25;break;case 0xd8:b->resource_id=26;break;
 case 0xa1:b->resource_id=21;break;case 0xd9:b->resource_id=29;break;case 0xa2:b->resource_id=18;break;case 0x6c:b->resource_id=20;break;
 case 0x6d:b->resource_id=11;break;case 0x6e:b->resource_id=15;b->subtype=9;break;case 0x6f:b->resource_id=17;b->subtype=1;break;
 case 0xcb:b->resource_id=23;b->subtype=11;break;case 0xcc:b->resource_id=12;b->subtype=5;b->secondary_resource_model=13;break;
 case 0xe8:b->resource_id=34;b->subtype=23;b->secondary_resource_model=24;break;case 0xe9:b->resource_id=33;b->subtype=20;break;
 case 0xcd:b->resource_id=28;b->subtype=4;break;case 0x70:b->resource_id=10;b->subtype=17;break;
 case 0x71:b->resource_id=19;b->subtype=10;break;case 0x72:b->resource_id=13;b->subtype=5;break;
 case 0xb1:b->resource_id=0;b->subtype=4;break;case 0xe7:b->resource_id=0;b->subtype=25;b->secondary_resource_model=5;break;
 case 0xb2:case 0xb3:b->resource_id=0;b->subtype=0;break;case 0x5f:b->resource_id=0;b->subtype=8;b->secondary_resource_model=14;break;
 case 0x4a:b->resource_id=0;b->subtype=4;pharaoh_building_repair_queue_add(c->surface->buildings,(int32_t)id,c->repair_queue,c->output_debug,c->debug_user);break;
 case 0xbb:case 0xbc:case 0xbd:b->resource_id=0;b->subtype=15;break;case 0x35:case 0x33:b->resource_id=0;b->subtype=12;break;
 case 0x2f:b->resource_id=0;b->subtype=16;break;case 0x20:b->resource_id=0;b->subtype=19;break;
 default:b->resource_id=0;if(type==0xe2){b->subtype=22;b->secondary_resource_model=13;}break;
 }
 type=b->building_type;if(type==0x47||type==0xbf)b->allocation_native_56=3200;
 cell=(c->surface->map->origin)+(uint32_t)y*228u+(uint32_t)x;b->x=(uint16_t)x;b->y=(uint16_t)y;b->grid_offset=cell;
 value=(c->surface->random[cell])&127u;b->reserved_45[8]=(uint8_t)value;b->reserved_32[2]=(uint8_t)(value&6u);b->reserved_45[7]=(c->definitions[kind].native_04);
 b->render_native_c0=(uint8_t)pharaoh_map_pattern_has_water(c->surface->map->terrain,c->surface->map->origin,x,y,c->water_patterns,b->size);
 if(b->building_type==136){
  value=(*c->ferry_state);if(!value)(*c->first_ferry)=id;
  else if(value==1||value==2){value=(*c->first_ferry);b->linked_ferry_building_id=value;c->surface->buildings[value].linked_ferry_building_id=id;}
 }
 type=b->building_type;if(type==31||type==30||type==33){
  b->bazaar_pottery_supply=0;b->bazaar_jewelry_supply=0;b->bazaar_linen_supply=0;b->bazaar_beer_supply=0;b->runtime_value_a7=0;b->runtime_value_a8=0;b->temple_upgrade=0;
 }
 type=b->building_type;
 if(type==115||type==108||type==177||type==178||type==179||type==231||type==195||floodplain_farm(c,(int32_t)id)||b->building_type==199)b->runtime_value_a6=0;
 if((int32_t)(*c->highest_id)<=(int32_t)id)(*c->highest_id)=id;
 return (int32_t)id;
failure:pharaoh_city_warning_post(c->warnings,65);return 0;
}
