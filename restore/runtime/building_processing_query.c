#include "building_processing_query.h"
#include "building_occupancy_query.h"
#include "menu_building_kind.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Full487F80 EAX keeps the high24 bits of the wrapped kind-100 DWORD. */
uint32_t restore_kind_is_processing_type(int32_t kind)
{
 uint32_t result=((uint32_t)kind-100u)&0xffffff00u;
 switch(kind){case 100:case 101:case 102:case 103:case 104:case 105:case 196:case 224:return result|1u;default:return result;}
}
uint32_t restore_building_is_processing_type(uint32_t building)
{return FUN_00487f80((int32_t)(int16_t)W(0x7999e8u+building*264u));}

/* The overlapping signed >=106/<=114 comparisons always clear native BL.
   Both switch arms then converge on the same permission WORD. Preserve both
   type reads and the genuine child call, including child side effects. */
uint32_t restore_building_resource_is_permitted(int32_t building)
{
 uint32_t record=(uint32_t)building*264u,owner,resource;volatile uint16_t kind;
 kind=W(0x7999e8u+record);(void)kind;(void)FUN_00488030((uint32_t)building);
 kind=W(0x7999e8u+record);(void)kind;
 owner=U(0xeaac28u);resource=B(0x799a1bu+record);
 return W(0xea4126u+owner*0x49d8u+resource*2u)!=0;
}

uint32_t restore_city_has_unpermitted_occupied_building(uint32_t self)
{
 uint32_t result=U(self+0x32fb4u),building,record;
 if(result==1u||result==2u||result==3u)return result&0xffffff00u;
 /* Native scans1..3999; the building high-water global is not a bound. */
 for(building=1;building<4000u;++building){
  record=building*264u;if(B(0x7999d8u+record)!=1u)continue;
  result=FUN_00488030(building);if(!(uint8_t)result)continue;
  result=FUN_004894a0((int32_t)building);if(!(uint8_t)result)continue;
  if(W(0x799a8cu+record))continue;
  result=FUN_00459240((int32_t)building);if(!result)return 1;
 }
 return result&0xffffff00u;
}
