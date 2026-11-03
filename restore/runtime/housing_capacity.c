#include "housing_capacity.h"
#include "menu_prepare_integer.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

void restore_building_decay_timers(void)
{
 uint32_t count=U(0x78d790u),id,record;int32_t kind,timer;
 if(I(count)<1)return;
 for(id=1;id<=count;++id){
  record=id*264u;if(!B(0x7999d8u+record))continue;
  kind=H(0x7999e8u+record);if(kind==59||kind==172||kind==173)continue;
  timer=H(0x7999f0u+record);W(0x7999f0u+record)=timer<=1?0:(uint16_t)(timer-1);
 }
}
void restore_housing_rebuild_list(void)
{
 uint32_t bound=U(0x78d790u),id,count=0,record;
 U(0x78d83cu)=0;if(I(bound)<1)return;
 for(id=1;id<=bound;++id){
  record=id*264u;
  if(B(0x7999d8u+record)==1u&&B(0x7999ddu+record)){W(0x78e178u+count*2u)=(uint16_t)id;++count;}
 }
 U(0x78d83cu)=count;
}
void restore_migration_reset_monthly(void)
{if(B(0xea624au)){U(0xea627cu)=0;U(0xea6280u)=0;}}
void restore_housing_rebuild_capacity(void)
{
 uint32_t count,index,record,capacity,off,population;
 FUN_004e8610();FUN_004583f0();count=U(0x78d83cu);U(0x78d840u)=0;if(I(count)<1)return;
 U(0x78d840u)=count;
 for(index=0;index<count;++index){
  record=(uint32_t)H(0x78e178u+index*2u)*264u;W(0x7999f6u+record)=0;
  if(H(0x7999f8u+record)<=0)continue;
  capacity=U(0x70ce14u+(uint32_t)H(0x7999eau+record)*88u);if(B(0x7999dcu+record))capacity*=4u;
  off=U(0xeaac28u)*0x49d8u;population=(uint32_t)H(0x7999f4u+record);
  U(0xea18a4u+off)=U(0xea18a4u+off)+capacity;U(0xea18a8u+off)=U(0xea18a8u+off)+capacity-population;
  if(!B(0x799a5fu+record))W(0x7999f6u+record)=(uint16_t)(capacity-population);
  if(H(0x7999f4u+record)>H(0x7999fau+record))W(0x7999fau+record)=W(0x7999f4u+record);
 }
}
void restore_migration_calculate_demand(int32_t player)
{
 uint32_t off=(uint32_t)player*0x49d8u,prior=U(0xea3f9cu+off),owner,value,delay;int32_t sentiment=I(U(0xea1880u+off)),rate;
 rate=sentiment>70?100:sentiment>60?75:sentiment>=50?50:sentiment>40?0:sentiment>30?-10:sentiment>20?-25:-50;
 U(0xea3f9cu+off)=(uint32_t)rate;owner=U(0xeaac28u)*0x49d8u;U(0xea3f5cu+off)=0;U(0xea3f58u+off)=0;
 if(I(U(0xea1894u+owner))>=200000){U(0xea3f9cu+off)=0;return;}
 value=U(0xea5eb4u+owner)+U(0xea5ea4u+owner);
 if(I(value)>3&&I(U(0xea3f9cu+off))>0){U(0xea3f9cu+off)=0;return;}
 value=U(0xea3f9cu+off);
 if(I(value)>0){
  delay=U(0xea3fa8u+off);if(delay){U(0xea3fa8u+off)=delay-1u;return;}
  U(0xea3f58u+off)=(uint32_t)FUN_004cde40(12,I(value));U(0xea3fa4u+off)=2;
 }else if(I(value)<0){
  delay=U(0xea3fa4u+off);if(delay){U(0xea3fa4u+off)=delay-1u;return;}
  if(I(U(0xea1894u+off))<=100)return;
  U(0xea3f5cu+off)=(uint32_t)FUN_004cde40(12,I(0u-value));U(0xea3fa8u+off)=2;
 }
 if(prior!=U(0xea3f9cu+off))FUN_004e2fb0(0);
}
