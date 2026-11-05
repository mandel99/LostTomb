#include "city_maintenance.h"
#include "building_footprint.h"
#include "building_removal.h"
#include "event_publisher.h"
#include "god_building_effects.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "menu_prepare_integer.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

/* Both inline native random advances shift two 31-bit feedback registers31
   times. History records the previous first stream's low7 result before the
   index increments; an overflowing signed index has the native wrap behavior. */
static uint32_t advance_hazard_random(uint32_t initial)
{
 uint32_t index=U(0xd3b1c4u),a,b,i,feedback_a,feedback_b;
 U(0xd3b1c8u+index*4u)=U(0xd3b374u);++index;U(0xd3b1c4u)=index;
 if(I(index)>=100)U(0xd3b1c4u)=0;
 a=U(0xd3b384u);b=U(0xd3b380u);
 for(i=0;i<31u;++i){
  feedback_a=((a>>4)^a)&1u;feedback_b=((b>>4)^b)&1u;
  a=(a>>1)|(feedback_a<<30);b=(b>>1)|(feedback_b<<30);
 }
 U(0xd3b384u)=a;U(0xd3b378u)=a&7u;U(0xd3b380u)=b;U(0xd3b364u)=b&0x7fffu;
 if(initial){U(0xd3b36cu)=b&7u;U(0xd3b370u)=a&0x7fffu;U(0xd3b374u)=a&0x7fu;U(0xd3b368u)=b&0x7fu;}
 else{U(0xd3b370u)=a&0x7fffu;U(0xd3b368u)=b&0x7fu;U(0xd3b36cu)=b&7u;U(0xd3b374u)=a&0x7fu;}
 return a;
}

/* Whole468B70. All record WORD arithmetic wraps before signed hazard tests.
   The initial random bucket stays fixed through the pass; each eligible
   non-collapsing root advances the stream even when it misses that bucket. */
void restore_city_update_building_hazards(void)
{
 uint32_t off=U(0xeaac28u)*0x49d8u,id=1u,record,kind,tile,weight,bucket,factor,table;
 uint32_t now,last,size,x,y,damage,increment,changed=0u;
 U(0xea5c84u+off)=0;U(0xea5c88u+off)=0;
 if(U(0x978d70u)==1u)return;
 bucket=advance_hazard_random(1u)&7u;
 if(I(U(0x78d790u))<1)return;
 do{
  record=id*264u;
  if(B(0x7999d8u+record)!=1u)goto next_building;
  kind=(uint32_t)H(0x7999e8u+record);
  if(kind==99u||W(0x799a22u+record)!=0u)goto next_building;
  if((kind==32u||kind==226u||kind==210u)&&W(0x799a10u+record)!=0u)goto next_building;
  if(kind==73u)goto next_building;
  if(I(kind)>=65&&I(kind)<=69&&W(0x799a10u+record)!=0u)goto next_building;
  if(FUN_00475b50(id)!=id)goto next_building;
  tile=U(0x7999e4u+record);weight=((uint32_t)B(0xacf85cu+tile)+id)&7u;
  table=(uint32_t)H(0x7999e8u+record)*40u;
  increment=W(0x705ec0u+table);damage=(uint16_t)(increment+W(0x799a1eu+record));
  W(0x799a1eu+record)=(uint16_t)damage;
  if(W(0x799a22u+record)!=0u){
   table=(uint32_t)H(0x7999e8u+record)*40u;
   increment=W(0x705ec0u+table);W(0x799a1eu+record)=(uint16_t)(damage+increment*10u);
  }
  if(B(0x7999ddu+record)!=0u&&H(0x7999eau+record)<=1)W(0x799a1eu+record)=0;
  tile=U(0x7999e4u+record);if(U(0xaf598cu+tile*4u)&0x10000u)W(0x799a1eu+record)=0;
  if(H(0x799a1eu+record)>=1000){
   now=U(0xe92e0cu);last=U(0xe950ecu);
   if(I(now-last)>15000)U(0xe950ecu)=now;else U(0xe950e0u)=0;
   if(B(0x7027bbu)==0u)FUN_00417ae0(3);
   else{
    if(I(U(0xe9512cu))<=0){U(0xe950dcu)=1;U(0xe9512cu)=12;}
    kind=(uint32_t)H(0x7999e8u+record);tile=U(0x7999e4u+record);
    FUN_004e0cf0(112,kind,tile,0,0);U(0xe9517cu)=U(0xe9517cu)+1u;
   }
   B(0x7999d8u+record)=4;kind=(uint32_t)H(0x7999e8u+record);U(0xbc9694u)=0;
   if(kind==202u)restore_building_clear_four_parts(I(id));
   else{
    y=(uint32_t)H(0x7999e0u+record);x=(uint32_t)H(0x7999deu+record);size=B(0x7999dbu+record);
    FUN_0047c2b0(I(id),I(x),I(y),I(size),I(size));
   }
   y=(uint32_t)H(0x7999e0u+record);x=(uint32_t)H(0x7999deu+record);size=B(0x7999dbu+record);
   restore_building_spawn_removal_effects(I(x),I(y),size,1);
   restore_building_clear_linked_parts(I(id),0);changed=1;goto next_building;
  }
  factor=(advance_hazard_random(0u)&0x7fffu)%5u+1u;
  if(weight!=bucket)goto next_building;
  if(B(0x7999ddu+record)!=0u){
   if(H(0x7999f4u+record)<=0)W(0x799a20u+record)=0;
   else{
    table=(uint32_t)H(0x7999e8u+record)*40u;
    W(0x799a20u+record)=(uint16_t)(W(0x799a20u+record)+W(0x705ebcu+table)*factor);
   }
  }else{
   if(B(0x784cb0u)==2u){
    table=(uint32_t)H(0x7999e8u+record)*40u;
    if(U(0x705ebcu+table)!=0u)W(0x799a20u+record)=(uint16_t)(W(0x799a20u+record)+3u);
   }
   table=(uint32_t)H(0x7999e8u+record)*40u;
   W(0x799a20u+record)=(uint16_t)(W(0x799a20u+record)+W(0x705ebcu+table)*factor);
   table=(uint32_t)H(0x7999e8u+record)*40u;
   if(U(0x705ebcu+table)==0u)W(0x799a20u+record)=0;
  }
  if(H(0x799a20u+record)<1000)goto next_building;
  now=U(0xe92e0cu);last=U(0xe950f0u);W(0x799a22u+record)=1;
  if(I(now-last)>15000){last=now;U(0xe950f0u)=last;}else U(0xe950e0u)=0;
  if(B(0x7027b8u)==0u)FUN_00417ae0(0);
  else{
   if(I(now-last)>15000)U(0xe950f0u)=now;else U(0xe950e0u)=0;
   if(I(U(0xe95128u))<=0){U(0xe950dcu)=1;U(0xe95128u)=12;}
   kind=(uint32_t)H(0x7999e8u+record);tile=U(0x7999e4u+record);
   FUN_004e0cf0(111,kind,tile,0,0);U(0xe95178u)=U(0xe95178u)+1u;
  }
  kind=(uint32_t)H(0x7999e8u+record);
  if(kind==31u||kind==30u||kind==33u||kind==209u||(I(kind)>=65&&I(kind)<=69))restore_city_mark_linked_fire(id);
  else{W(0x799a22u+record)=1;restore_god_building_0046d100(I(id),0);restore_building_clear_linked_parts(I(id),1);}
next_building:
  ++id;
 }while(I(id)<=I(U(0x78d790u)));
 if(changed){FUN_00517370();FUN_00517660();}
}
