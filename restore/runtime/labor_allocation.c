#include "labor_allocation.h"
#include "displaced_population.h"
#include "actor_teardown_model.h"
#include "production_updates.h"
#include "menu_list_scroll.h"
#include "map_commit_connect.h"
#include "map_commit_links.h"
#include "event_publisher.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

void restore_housing_displace_overcrowding(void)
{
 uint32_t count=U(0x78d83cu),index=0,record;int32_t space,people;
 U(0x78d840u)=0;if(I(count)<=0)return;
 do{
  record=(uint32_t)H(0x78e178u+index*2u)*264u;U(0x78d840u)=++index;space=H(0x7999f6u+record);
  if(space<0){
   people=-space;FUN_00459020(I(U(0xeaac28u)),H(0x7999deu+record),H(0x7999e0u+record),people);
   if(people<H(0x7999f4u+record))W(0x7999f4u+record)=(uint16_t)(H(0x7999f4u+record)-people);else B(0x7999d8u+record)=2;
  }
  index=U(0x78d840u);count=U(0x78d83cu);
 }while(I(index)<I(count));
}
void restore_labor_rebuild_demand(void)
{
 uint32_t off=U(0xeaac28u)*0x49d8u,index,id,record,at;int32_t group,coverage;
 for(index=0;index<10u;++index){at=off+index*20u;U(0xea43a8u+at)=0;U(0xea43a4u+at)=0;U(0xea43a0u+at)=0;U(0xea439cu+at)=0;}
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u)continue;
  group=I(U(0x5d06ecu+(uint32_t)H(0x7999e8u+record)*20u));
  if(!group&&(uint8_t)restore_actor_building_gate(I(id)))group=-1;
  B(0x799a1au+record)=(uint8_t)group;if(group<0)continue;
  if(group==3&&W(0x799a10u+record))continue;
  if((group==0||group==1)&&FUN_00459240(I(id)))continue;
  coverage=H(0x7999f0u+record);if(coverage<=0)continue;
  at=U(0xeaac28u)*0x49d8u+(uint32_t)group*20u;
  U(0xea439cu+at)=U(0xea439cu+at)+U(0x705eb8u+(uint32_t)H(0x7999e8u+record)*40u);
  U(0xea43a4u+at)=U(0xea43a4u+at)+(uint32_t)coverage;U(0xea43a8u+at)=U(0xea43a8u+at)+1u;
 }
}
void restore_labor_allocate_groups(void)
{
 uint32_t off=U(0xeaac28u)*0x49d8u,index,at,total=0,left,rank,round,table,take,current,space,free_people;int32_t weight;
 for(index=0;index<10u;++index)U(0xea43a0u+off+index*20u)=0;
 for(index=0;index<10u;++index)total+=U(0xea439cu+off+index*20u);
 U(0xea4474u+off)=0;left=U(0xea4398u+off);
 if(I(total)>I(left)){
  for(rank=1;;++rank){
   for(index=0;index<9u;++index){
    at=off+index*20u;if(U(0xea43acu+at)!=rank)continue;
    take=U(0xea439cu+at);if(I(take)>I(left))take=left;U(0xea43a0u+at)=take;left-=take;break;
   }
   if(I(left)<=0||rank>=9u)break;
  }
  round=1;
  if(I(U(0xea4398u+off))>1)while(I(left)>0){
   for(table=0x5da450u;table<0x5da498u;table+=8u){
    at=off+U(table-4u)*20u;if(U(0xea43acu+at))continue;
    current=U(0xea43a0u+at);space=U(0xea439cu+at)-current;if(I(space)<=0)continue;
    weight=I(U(table));take=I(left)<weight?left:(uint32_t)weight;if(I(take)>I(space))take=space;
    left-=take;U(0xea43a0u+at)=current+take;if(I(left)<=0)break;
   }
   if(I(left)<=0)break;
   ++round;if(I(round)>=I(U(0xea4398u+off)))break;
  }
  U(0xea4464u+off)=U(0xea4398u+off);
  for(index=0;index<9u;++index)U(0xea4474u+off)=U(0xea4474u+off)+U(0xea439cu+off+index*20u)-U(0xea43a0u+off+index*20u);
 }else{
  for(index=0;index<10u;++index)U(0xea43a0u+off+index*20u)=U(0xea439cu+off+index*20u);
  U(0xea4464u+off)=total;
 }
 free_people=U(0xea4398u+off)-U(0xea4464u+off);U(0xea4468u+off)=free_people;
 total=(uint32_t)FUN_004cde60(I(free_people),I(U(0xea4398u+off)));U(0xea446cu+U(0xeaac28u)*0x49d8u)=total;
}
void restore_labor_update_unemployment(void)
{
 uint32_t prior=U(0xea4474u+U(0xeaac28u)*0x49d8u),off,value,old;
 FUN_00459400();off=U(0xeaac28u)*0x49d8u;value=U(0xea446cu+off);old=U(0xea4470u+off);
 U(0xea4470u+off)=(I(value)<I(old)||I(value-5u)<I(old))?value:old+5u;
 if(I(U(0xea4470u+off))>100)U(0xea4470u+off)=100;
 if(!prior&&I(U(0xea4474u+off))>0&&I(U(0x78d7bcu))>=H(0x7848d8u)&&I(U(0xe95144u))<=0){U(0xe95144u)=6;FUN_004e0cf0(136,0,0,0,0);}
}
void restore_labor_rebuild_shares(void)
{
 uint32_t id,record,off,denominator;int32_t group,coverage;
 (void)FUN_004cde60(100,I(U(0xea4448u+U(0xeaac28u)*0x49d8u)));
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u)continue;
  group=I(U(0x5d06ecu+(uint32_t)H(0x7999e8u+record)*20u));
  if(!group){if((uint8_t)restore_actor_building_gate(I(id)))continue;}else if(group==3){if(W(0x799a10u+record))continue;}else if(group<0)continue;
  off=U(0xeaac28u)*0x49d8u;W(0x7999f2u+record)=0;denominator=U(0xea43a4u+off+(uint32_t)group*20u);coverage=H(0x7999f0u+record);
  if(coverage)W(0x7999f2u+record)=(uint16_t)FUN_004cde60(coverage*100,I(denominator));
 }
}
void restore_labor_assign_buildings(void)
{
 uint32_t owner=U(0xeaac28u),off=owner*0x49d8u,left[10],index,id,record,cached,at,amount,needed,max,bound;int32_t group,kind,share,workers;
 for(index=0;index<10u;++index)left[index]=I(U(0xea43a0u+off+index*20u))<I(U(0xea439cu+off+index*20u));
 bound=U(0x78d790u);
 if(I(bound)>=1){
  for(id=1;;++id){
   record=id*264u;if(B(0x7999d8u+record)!=1u)goto next_first;
   group=I(U(0x5d06ecu+(uint32_t)H(0x7999e8u+record)*20u));
   if((group==0||group==1)&&(uint8_t)restore_actor_building_gate(I(id)))goto next_first;
   if(group<0||(group==3&&W(0x799a10u+record)))goto next_first;
   W(0x799a18u+record)=0;
   if(group==0||group==1){if(FUN_00459240(I(id)))goto next_first;}
   else if(group==2){kind=H(0x7999e8u+record);if((kind==30||kind==31||kind==33)&&W(0x799a10u+record))goto next_first;}
   else if(group==8){kind=H(0x7999e8u+record);if((kind==112||kind==205)&&FUN_00459240(I(id)))goto next_first;}
   share=H(0x7999f2u+record);if(share<=0)goto next_first;cached=left[group];
   if(!cached)W(0x799a18u+record)=W(0x705eb8u+(uint32_t)H(0x7999e8u+record)*40u);
   else{
    amount=(uint32_t)(FUN_004cde40(I(U(0xea43a0u+U(0xeaac28u)*0x49d8u+(uint32_t)group*20u)),share)/100);
    max=U(0x705eb8u+(uint32_t)H(0x7999e8u+record)*40u);if(I(amount)>=I(max))amount=max;cached+=amount;
    W(0x799a18u+record)=(uint16_t)amount;left[group]=cached;
   }
 next_first:bound=U(0x78d790u);if(I(id+1u)>I(bound))break;
  }
  owner=U(0xeaac28u);
 }
 for(index=0;index<10u;++index)if(left[index])--left[index];
 off=owner*0x49d8u;
 for(index=0;index<10u;++index){amount=U(0xea43a0u+off+index*20u);left[index]=I(left[index])<I(amount)?amount-left[index]:0;}
 if(I(bound)>=1)for(id=1;;++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u)goto next_second;
  group=I(U(0x5d06ecu+(uint32_t)H(0x7999e8u+record)*20u));
  if(!group){if((uint8_t)restore_actor_building_gate(I(id)))goto next_second;}
  else if(group<0||group==8)goto next_second;
  else if(group==2&&W(0x799a10u+record))goto next_second;
  if((group==0||group==1)&&FUN_00459240(I(id)))goto next_second;
  if(H(0x7999f2u+record)<=0||!left[group])goto next_second;
  workers=H(0x799a18u+record);max=U(0x705eb8u+(uint32_t)H(0x7999e8u+record)*40u);if(workers>=I(max))goto next_second;
  needed=max-(uint32_t)workers;
  if(I(needed)>I(left[group])){amount=(uint32_t)workers+left[group];left[group]=0;W(0x799a18u+record)=(uint16_t)amount;}
  else{cached=left[group]-needed;W(0x799a18u+record)=(uint16_t)((uint32_t)workers+needed);left[group]=cached;}
 next_second:bound=U(0x78d790u);if(I(id+1u)>I(bound))break;
 }
 if(I(bound)>=1)for(id=1;;++id){
  record=id*264u;
  if(B(0x7999d8u+record)==1u&&W(0x7999e8u+record)==136u){
   at=U(0x799a9cu+record)*264u;
   if(W(0x799a18u+record)&&H(0x799a18u+at)>0)FUN_0043f060(I(id),1,1);else{FUN_0043f060(I(id),1,3);FUN_004a36b0(I(id));}
  }
  bound=U(0x78d790u);if(I(id+1u)>I(bound))break;
 }
}
