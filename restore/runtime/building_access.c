#include "building_access.h"
#include "actor_order_flood.h"
#include "actor_order_path.h"
#include "camera_navigation.h"
#include "canal_region_model.h"
#include "city_updates.h"
#include "command_migration.h"
#include "displaced_population.h"
#include "event_publisher.h"
#include "map_commit_costs.h"
#include "menu_prepare_integer.h"
#include "monument_entrance.h"
#include "monument_complex_positions.h"
#include "placement_support.h"
#include "remaining_menu_actions.h"
#include "surface_passes.h"
#include "terrain_redraw.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))
extern uint32_t FUN_00542f40(int32_t,int32_t *,int32_t *);
extern uint32_t FUN_004e0c50(int32_t,int32_t *,int32_t *);
extern void FUN_00517370(void);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);

/* Whole481420: a zero offset terminates this24-slot table immediately. */
uint32_t restore_building_has_access_neighbor(int32_t x,int32_t y,int32_t size)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 uint32_t at=0x5cc9e0u+(uint32_t)size*96u,index,delta,neighbor;
 for(index=0;index<24u;++index,at+=4u){
  delta=U(at);if(!delta)return 0;
  neighbor=cell+delta;
  if((B(0xaf598cu+neighbor*4u)&4u)&&U(0xeed760u+neighbor*4u))return 1;
 }
 return 0;
}

void restore_building_refresh_route_flags(void)
{
 uint32_t id,record;uint16_t kind;
 (void)FUN_0051ce90(H(0x784c8cu),H(0x784c8eu));
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;
  if(B(0x7999d8u+record)!=1u||B(0x7999ddu+record))continue;
  kind=W(0x7999e8u+record);if(kind!=75u&&kind!=192u)continue;
  B(0x799a0du+record)=FUN_00481420(H(0x7999deu+record),H(0x7999e0u+record),3)!=0;
 }
}

/* Native common return from the three neighbor scans uses result-1 for both
   color and distance. The DWORD zero test and subsequent WORD publication
   are intentionally distinct. These fields can overlap other query outputs. */
static void publish_neighbor(uint32_t record,uint32_t result,uint32_t late_position)
{
 uint32_t tile=result-1u;uint8_t color=B(0xa76aecu+tile);
 uint16_t distance=W(0xeed760u+tile*4u),x,y;
 B(0x7999ecu+record)=color;
 if(late_position){
  W(0x7999f8u+record)=distance;x=W(0xd3b4e4u);y=W(0xd3b4e0u);
  W(0x7999feu+record)=x;W(0x799a00u+record)=y;return;
 }
 y=W(0xd3b4e0u);x=W(0xd3b4e4u);
 W(0x7999f8u+record)=distance;
 W(0x7999feu+record)=x;W(0x799a00u+record)=y;
}
/* Native object-query result path caches X/Y before its BYTE/WORD stores. */
static void publish_position(uint32_t record,uint32_t early_distance)
{
 uint32_t y=U(0xd3b4e0u),x=U(0xd3b4e4u),tile=U(0xbca2d8u)+y*228u+x;
 uint16_t distance=0;
 if(early_distance)distance=W(0xeed760u+tile*4u);
 B(0x7999ecu+record)=B(0xa76aecu+tile);
 if(!early_distance)distance=W(0xeed760u+tile*4u);
 W(0x7999f8u+record)=distance;
 W(0x7999feu+record)=(uint16_t)x;W(0x799a00u+record)=(uint16_t)y;
}
static void publish_coordinates(uint32_t record)
{
 uint16_t x=W(0xd3b4e4u),y=W(0xd3b4e0u);
 W(0x7999feu+record)=x;W(0x799a00u+record)=y;
}

void restore_building_update_access(void)
{
 uint32_t off=U(0xeaac28u)*0x49d8u,id,record,result,tile,pending=0,linked,attempt,at;
 uint16_t kind,timer;uint8_t empty,ok;
 (void)FUN_00517a40(H(0xea3fe8u+off),H(0xea3feau+off),-2,-2);
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u)continue;
  if(B(0x7999ddu+record)){
   result=FUN_0047fe60(H(0x7999deu+record),H(0x7999e0u+record),B(0x7999dbu+record),2);
   if(!result){
    W(0x7999f8u+record)=0;timer=(uint16_t)(W(0x7999fcu+record)+1u);W(0x7999fcu+record)=timer;
    if((int16_t)timer>4){
     if(W(0x7999f4u+record)){
      FUN_00459020(I(U(0xeaac28u)),H(0x7999deu+record),H(0x7999e0u+record),H(0x7999f4u+record));
      W(0x7999f4u+record)=0;W(0x7999fcu+record)=0;
     }
     B(0x7999d8u+record)=2;
    }
   }else{
    tile=U(0xbc9768u);
    if(!U(0xeed760u+tile*4u)){
     W(0x7999f8u+record)=0;empty=W(0x7999e8u+record)==10u&&W(0x7999f4u+record)==0;
     timer=W(0x7999fcu+record);
     if(!timer&&(empty||!pending))pending=U(0x7999e4u+record);
     timer=(uint16_t)(timer+1u);W(0x7999fcu+record)=timer;
     if((int16_t)timer>8){W(0x7999fcu+record)=0;if(empty)B(0x7999d8u+record)=2;}
    }else{W(0x7999f8u+record)=W(0xeed760u+tile*4u);W(0x7999fcu+record)=0;}
   }
   continue;
  }
  kind=W(0x7999e8u+record);
  if(kind==72u||kind==193u){
   off=U(0xeaac28u)*0x49d8u;if(!U(0xea5ee4u+off))U(0xea5ee4u+off)=id;
   W(0x7999f8u+record)=0;
   result=restore_building_access_neighbor_query(0x481060u,H(0x7999deu+record),H(0x7999e0u+record),3);
   if(result)publish_neighbor(record,result,1);
   continue;
  }
  W(0x7999f8u+record)=0;
  if(kind==210u){
   result=restore_building_access_neighbor_query(0x487d00u,H(0x7999deu+record),H(0x7999e0u+record),5);
   if(result)publish_neighbor(record,result,0);
  }else if(kind==213u){
   if((uint8_t)FUN_00542f40(I(id),(int32_t *)(uintptr_t)0xd3b4e4u,(int32_t *)(uintptr_t)0xd3b4e0u)){
    publish_coordinates(record);
   }
  }else if(kind==225u){
   if((uint8_t)restore_monument_entrance_0040eb90(0x6fd5d8u,id,0xd3b4e4u,0xd3b4e0u))publish_position(record,1);
  }else if(kind==230u){
   if((uint8_t)restore_monument_entrance_00401af0(id,0xd3b4e4u,0xd3b4e0u))publish_position(record,0);
  }else if(kind==227u){
   if((uint8_t)restore_monument_entrance_00417490(id,0xd3b4e4u,0xd3b4e0u))publish_position(record,0);
  }else if(kind==228u){
   if((uint8_t)restore_monument_entrance_004e6ae0(id,0xd3b4e4u,0xd3b4e0u))publish_position(record,0);
  }else if(kind==222u){
   if((uint8_t)FUN_004e0c50(I(id),(int32_t *)(uintptr_t)0xd3b4e4u,(int32_t *)(uintptr_t)0xd3b4e0u)){
    publish_coordinates(record);
   }
  }else if(kind==229u||kind==234u||kind==235u||kind==236u){
   if(kind==229u)ok=(uint8_t)restore_complex_position_00545a30(id,0xd3b4e4u,0xd3b4e0u);
   else if(kind==234u)ok=(uint8_t)restore_complex_position_00558f00(id,0xd3b4e4u,0xd3b4e0u);
   else if(kind==235u)ok=(uint8_t)restore_complex_position_00556230(id,0xd3b4e4u,0xd3b4e0u);
   else ok=(uint8_t)restore_complex_position_0054f8a0(id,0xd3b4e4u,0xd3b4e0u);
   if(ok)publish_position(record,1);
  }else if((kind>=65u&&kind<=69u)||(kind>=156u&&kind<=160u)){
   result=restore_building_access_neighbor_query(0x481210u,H(0x7999deu+record),H(0x7999e0u+record),3);
   if(result)publish_neighbor(record,result,0);
  }else if(kind==73u){
   linked=FUN_00475b50(id)*264u;
   B(0x7999ecu+record)=B(0x7999ecu+linked);W(0x7999f8u+record)=W(0x7999f8u+linked);
   W(0x7999feu+record)=W(0x7999feu+linked);W(0x799a00u+record)=W(0x799a00u+linked);
  }else if(kind==183u){
   if(restore_building_access_special_query(I(id),&tile,(int32_t *)(uintptr_t)0xd3b4e4u,(int32_t *)(uintptr_t)0xd3b4e0u))publish_neighbor(record,tile+1u,1);
  }else{
   result=restore_building_access_neighbor_query(0x481060u,H(0x7999deu+record),H(0x7999e0u+record),B(0x7999dbu+record));
   if(result)publish_neighbor(record,result,0);
  }
 }
 off=U(0xeaac28u)*0x49d8u;tile=U(0xea3ff4u+off);
 if(U(0xeed760u+tile*4u)){
  if(pending){restore_remaining_message_4e2970(63,0,0);restore_remaining_message_4e2970(64,0,0);FUN_00430cd0(pending);}
  return;
 }
 for(attempt=0;attempt<15u;++attempt){
  (void)FUN_00517c20(H(0xea3fe8u+off),H(0xea3feau+off),-2,-2);
  off=U(0xeaac28u)*0x49d8u;(void)FUN_00517c20(H(0xea3ff0u+off),H(0xea3ff2u+off),-2,-2);
  FUN_00517370();FUN_00517660();FUN_005178e0();FUN_0047b1a0();
  FUN_0047bae0(0,0,I(U(0xbca2d0u)-1u),I(U(0xbca2d4u)-1u),0);
  FUN_004767d0(0,0,I(U(0xbca2d0u)-1u),I(U(0xbca2d4u)-1u),0,0);
  FUN_00477500(0,0,I(U(0xbca2d0u)-1u),I(U(0xbca2d4u)-1u));FUN_00476530();
  off=U(0xeaac28u)*0x49d8u;(void)FUN_00517a40(H(0xea3fe8u+off),H(0xea3feau+off),-2,-2);
  off=U(0xeaac28u)*0x49d8u;at=U(0xea3ff4u+off);
  if(U(0xeed760u+at*4u)){
   U(0xe950dcu)=1;FUN_004e0cf0(315,0,0,0,0);U(0xbc9694u)=0;return;
  }
 }
 restore_building_access_remove_disconnected();
}
