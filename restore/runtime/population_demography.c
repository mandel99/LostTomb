#include "population_demography.h"
#include "population_updates.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

int32_t restore_population_add_births(int32_t player,int32_t requested)
{
 uint32_t at=0xea3f88u+(uint32_t)player*0x49d8u,cursor=U(at),done=0,pass,record,capacity;int32_t population;uint8_t large;
 for(pass=1;pass<4000u;++pass){
  if(I(done)>=requested)break;
  ++cursor;if(I(cursor)>=4000)cursor=1;record=cursor*264u;
  if(B(0x7999d8u+record)!=1u||!B(0x7999ddu+record)||H(0x7999f8u+record)<=0)continue;
  population=H(0x7999f4u+record);if(population<=0)continue;
  U(at)=cursor;large=B(0x7999dcu+record);capacity=U(0x70ce14u+(uint32_t)H(0x7999eau+record)*88u);if(large)capacity*=4u;
  if(I(capacity-(uint32_t)population)<=0)continue;
  ++done;++population;W(0x7999f4u+record)=(uint16_t)population;W(0x7999f6u+record)=(uint16_t)(capacity-(uint32_t)population);
 }
 return I(done);
}
int32_t restore_population_remove_deaths(int32_t player,int32_t requested)
{
 uint32_t at=0xea3f8cu+(uint32_t)player*0x49d8u,cursor=(uint32_t)H(at),done=0,pass,record;int32_t population;
 for(pass=1;pass<16000u;++pass){
  if(I(done)>=requested)break;
  ++cursor;if(I(cursor)>=4000)cursor=1;record=cursor*264u;
  if(B(0x7999d8u+record)!=1u||!B(0x7999ddu+record))continue;
  population=H(0x7999f4u+record);W(at)=(uint16_t)cursor;
  if(population>0){++done;W(0x7999f4u+record)=(uint16_t)(population-1);}
 }
 return I(done);
}
int32_t restore_population_age_group(int32_t player,int32_t group)
{
 uint32_t at=0xea3e34u+(uint32_t)player*0x49d8u+(uint32_t)group*20u,index,total=0;
 for(index=0;index<10u;++index)total+=(uint32_t)H(at+index*2u);
 return I(total);
}
void restore_population_remove_age_group(int32_t player,int32_t group,int32_t amount)
{
 uint32_t at=0xea3e34u+(uint32_t)player*0x49d8u+(uint32_t)group*20u,index=0,empty=0,remaining=(uint32_t)amount;int32_t population;
 while(remaining&&empty<10u){
  population=H(at+index*2u);
  if(population>0){--remaining;W(at+index*2u)=(uint16_t)(population-1);empty=0;}else ++empty;
  if(++index>=10u)index=0;
 }
}
void restore_population_age_and_deaths(void)
{
 uint32_t player=U(0xeaac28u),off=player*0x49d8u,index,carry=(uint32_t)H(0xea3efau+off),removed,count,rate;int32_t group,bucket;
 for(index=99;index;--index)W(0xea3e34u+off+index*2u)=W(0xea3e32u+off+index*2u);
 W(0xea3e34u+off)=0;U(0xea3f50u+off)=0;
 for(group=9;group>=0;--group){
  count=(uint32_t)FUN_004e92c0(I(player),group);bucket=I(U(0xea1888u+U(0xeaac28u)*0x49d8u))/10;
  rate=U(0x5e49d4u+((uint32_t)group+(uint32_t)bucket*10u)*4u);
  count=(uint32_t)FUN_004cde40(I(count),I(rate));removed=(uint32_t)FUN_00458f00(I(U(0xeaac28u)),I(count+carry));
  FUN_004e92f0(I(U(0xeaac28u)),group,I(removed));player=U(0xeaac28u);off=player*0x49d8u;
  U(0xea3f50u+off)=U(0xea3f50u+off)+removed;carry=0;
 }
}
void restore_population_births(void)
{
 uint32_t player=U(0xeaac28u),off=player*0x49d8u,count,born;int32_t group;
 U(0xea3f4cu+off)=0;
 for(group=9;group>=0;--group){
  count=(uint32_t)FUN_004e92c0(I(player),group);count=(uint32_t)FUN_004cde40(I(count),I(U(0x5e49acu+(uint32_t)group*4u)));
  born=(uint32_t)FUN_00458e20(I(U(0xeaac28u)),I(count));player=U(0xeaac28u);off=player*0x49d8u;
  W(0xea3e34u+off)=(uint16_t)(W(0xea3e34u+off)+born);U(0xea3f4cu+off)=U(0xea3f4cu+off)+born;
 }
}
void restore_population_rebuild_totals(int32_t player)
{
 uint32_t off=(uint32_t)player*0x49d8u,index,total=0,period;
 U(0xea3f84u+off)=0;U(0xea1898u+off)=U(0xea1894u+off);U(0xea1894u+off)=0;
 for(index=0;index<100u;++index){total=U(0xea1894u+off)+(uint32_t)H(0xea3e34u+off+index*2u);U(0xea1894u+off)=total;}
 if(I(total)>I(U(0xea5f0cu+off)))U(0xea5f0cu+off)=total;
 U(0xea3f54u+off)=0;U(0xea3f74u+off)=U(0xea3f74u+off)+U(0xea1894u+off);
 period=U(0xea3f80u+off)+1u;U(0xea3f80u+off)=period;U(0xea3f70u+off)=(uint32_t)(I(U(0xea3f74u+off))/I(period));
}
int32_t restore_population_working_age(void)
{
 uint32_t total=(uint32_t)FUN_004e92c0(I(U(0xeaac28u)),2);
 total+=(uint32_t)FUN_004e92c0(I(U(0xeaac28u)),3);total+=(uint32_t)FUN_004e92c0(I(U(0xeaac28u)),4);return I(total);
}
void restore_population_rebuild_housing_mix(void)
{
 uint32_t count=U(0x78d83cu),index,record,low=0,high=0,result,off;int32_t population;
 U(0x78d840u)=0;
 if(I(count)>0){
  U(0x78d840u)=count;
  for(index=0;index<count;++index){
   record=(uint32_t)H(0x78e178u+index*2u)*264u;population=H(0x7999f4u+record);
   if(population>0){if(H(0x7999eau+record)<14)low+=(uint32_t)population;else high+=(uint32_t)population;}
  }
 }
 result=(uint32_t)FUN_004cde60(I(low),I(low+high));U(0xea4390u+U(0xeaac28u)*0x49d8u)=result;
 result=(uint32_t)FUN_004e9460();off=U(0xeaac28u)*0x49d8u;U(0xea4394u+off)=result;
 result=(uint32_t)FUN_004cde40(I(result),60);off=U(0xeaac28u)*0x49d8u;U(0xea4394u+off)=result;
 result=(uint32_t)FUN_004cde40(I(result),I(U(0xea4390u+off)));U(0xea4398u+U(0xeaac28u)*0x49d8u)=result;
}
