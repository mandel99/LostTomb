#include "migration_flow.h"
#include "housing_capacity.h"
#include "population_demography.h"
#include "population_updates.h"
#include "actor_create.h"
#include "building_support.h"
#include "event_publisher.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

void restore_migration_create_immigrants(int32_t building,uint8_t people)
{
 uint32_t player=U(0xeaac28u),off=player*0x49d8u,actor,record=(uint32_t)building*264u,figure;uint8_t appearance;
 actor=(uint32_t)FUN_004b4800((uint8_t)player,1,H(0xea3fe8u+off),H(0xea3feau+off),0);
 W(0x799a06u+record)=(uint16_t)actor;appearance=B(0x799a25u+record)&127u;figure=actor*388u;
 B(0xbcd630u+figure)=8;W(0xbcd654u+figure)=(uint16_t)building;W(0xbcd62eu+figure)=(uint16_t)(appearance+10u);B(0xbcd65cu+figure)=people;
}
void restore_migration_create_emigrants(int32_t building,int32_t people)
{
 uint32_t record=(uint32_t)building*264u,actor;int32_t population;
 FUN_004e90f0(I(U(0xeaac28u)),I(0u-(uint32_t)people));population=H(0x7999f4u+record);
 if(people<population)W(0x7999f4u+record)=(uint16_t)((uint32_t)population-(uint32_t)people);
 else{W(0x7999f4u+record)=0;FUN_0046cf40(building);}
 actor=(uint32_t)FUN_004b4800((uint8_t)U(0xeaac28u),2,H(0x7999deu+record),H(0x7999e0u+record),0)*388u;
 B(0xbcd630u+actor)=8;W(0xbcd62eu+actor)=0;B(0xbcd65cu+actor)=(uint8_t)people;
}
void restore_migration_allocate_immigrants(int32_t requested)
{
 uint32_t count=U(0x78d83cu),index=0,record,off,total,remaining=(uint32_t)requested;int32_t building,actor,space;
 if(I(count)>0&&I(remaining)>0)do{
  building=H(0x78e178u+index*2u);++index;record=(uint32_t)building*264u;actor=H(0x799a06u+record);
  if(actor&&B(0xbcd606u+(uint32_t)actor*388u)!=1u)W(0x799a06u+record)=0;
  count=U(0x78d83cu);
 }while(I(index)<I(count));
 /* The empty-house pass in the native CFG cannot execute: positive requests
    exhaust the cleanup scan; nonpositive requests fail its remaining gate. */
 index=0;
 if(I(count)>0)while(I(remaining)>0){
  building=H(0x78e178u+index*2u);++index;record=(uint32_t)building*264u;U(0x78d840u)=index;
  if(H(0x7999f8u+record)>0&&H(0x7999f6u+record)>=8&&!W(0x799a06u+record)){
   if(I(remaining)>4){FUN_00458d00(building,4);remaining-=4u;}else{FUN_00458d00(building,(uint8_t)remaining);remaining=0;}
   index=U(0x78d840u);
  }
  count=U(0x78d83cu);if(I(index)>=I(count))break;
 }
 index=0;U(0x78d840u)=0;
 if(I(count)>0)while(I(remaining)>0){
  building=H(0x78e178u+index*2u);++index;record=(uint32_t)building*264u;U(0x78d840u)=index;
  if(H(0x7999f8u+record)>0){
   space=H(0x7999f6u+record);
   if(space>0&&!W(0x799a06u+record)){
    if(I(remaining)>space){FUN_00458d00(building,(uint8_t)space);remaining-=(uint32_t)space;}else{FUN_00458d00(building,(uint8_t)remaining);remaining=0;}
   }
  }
  index=U(0x78d840u);count=U(0x78d83cu);if(I(index)>=I(count))break;
 }
 off=U(0xeaac28u)*0x49d8u;total=U(0xea3f90u+off)+(uint32_t)requested-remaining;U(0xea3f90u+off)=total;
 U(0xea3facu+off)=U(0xea3facu+off)+total;
 if(remaining&&(uint32_t)requested==remaining)U(0xea3f98u+off)=U(0xea3f98u+off)+remaining;
}
void restore_migration_allocate_emigrants(int32_t requested)
{
 uint32_t remaining=(uint32_t)requested,rank,index,count,record,off;int32_t building,people;
 for(rank=0;I(remaining)>0&&rank<10u;++rank){
  count=U(0x78d83cu);index=0;U(0x78d840u)=0;
  if(I(count)>0)while(I(remaining)>0){
   building=H(0x78e178u+index*2u);++index;record=(uint32_t)building*264u;U(0x78d840u)=index;people=H(0x7999f4u+record);
   if(people>0&&H(0x7999eau+record)==I(rank)){
    if(people>=4)people=4;
    if(I(remaining)>people){FUN_00458d80(building,people);remaining-=(uint32_t)people;}else{FUN_00458d80(building,I(remaining));remaining=0;}
   }
   index=U(0x78d840u);count=U(0x78d83cu);if(I(index)>=I(count))break;
  }
 }
 off=U(0xeaac28u)*0x49d8u;U(0xea3f94u+off)=U(0xea3f94u+off)+(uint32_t)requested-remaining;
}
void restore_migration_update(void)
{
 static const int32_t milestone[9]={500,1000,2000,3000,5000,10000,15000,20000,25000};
 uint32_t off,value,current,index;
 FUN_004e8f80(I(U(0xeaac28u)));off=U(0xeaac28u)*0x49d8u;U(0xea3f90u+off)=0;U(0xea3f94u+off)=0;U(0xea3f98u+off)=0;
 value=U(0xea3f58u+off);
 if(I(value)>0){
  if(I(value)>=4)FUN_00458950(I(value));
  else{value+=U(0xea3f64u+off);if(I(value)>=4){FUN_00458950(I(value));U(0xea3f64u+U(0xeaac28u)*0x49d8u)=0;}else U(0xea3f64u+off)=value;}
 }
 off=U(0xeaac28u)*0x49d8u;value=U(0xea3f5cu+off);
 if(I(value)>0){
  if(I(value)>=4)FUN_00458b60(I(value));
  else{value+=U(0xea3f60u+off);if(I(value)>=4){FUN_00458b60(I(value));U(0xea3f60u+U(0xeaac28u)*0x49d8u)=0;}else U(0xea3f60u+off)=value;}
 }
 off=U(0xeaac28u)*0x49d8u;U(0xea3f58u+off)=0;U(0xea3f5cu+off)=0;
 if(U(0xea3f84u+off)){FUN_004e8650();FUN_004e8720();FUN_004e87a0(I(U(0xeaac28u)));}
 FUN_00458880();current=U(0xeaac28u);
 for(index=0;index<9u;++index)if(I(U(0xea1894u+current*0x49d8u))>=milestone[index]&&!B(0xe951c5u+index)){
  U(0xe950dcu)=1;FUN_004e0cf0((int32_t)(101u+index),0,0,0,0);if(index<8u)current=U(0xeaac28u);B(0xe951c5u+index)=1;
 }
}
