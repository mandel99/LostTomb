#include "map_commit_population.h"
#include "actor_teardown_model.h"
#include "menu_building_kind.h"
#include "actor_predicate.h"
#include "map_refresh_arithmetic.h"
#include "map_x87_conversion.h"
#include "pending_message.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_004cde40(int32_t value,int32_t percentage);
extern int32_t FUN_004cde60(int32_t value,int32_t maximum);
static int32_t truncate(MRFloat value)
{
 uint8_t raw[10];mr_fp_raw80(value,raw);
 return (int32_t)(uint32_t)restore_map_trunc80(raw);
}
static int32_t divide(int32_t value,int32_t divisor)
{
 if(!divisor||(value==INT32_MIN&&divisor==-1))abort();
 return value/divisor;
}
uint32_t restore_building_flag_00488080(int32_t building)
{
 uint32_t kind=(uint32_t)(int32_t)H(0x7999e8u+(uint32_t)building*264u)-100u;
 uint32_t result=kind&0xffffff00u;
 switch(kind){case 0:case 1:case 2:case 3:case 4:case 5:case 96:case 124:return result|1u;default:return result;}
}
uint32_t restore_building_flag_00489880(int32_t building)
{
 uint32_t kind=(uint32_t)(int32_t)H(0x7999e8u+(uint32_t)building*264u)-100u;
 uint32_t result=kind&0xffffff00u;
 switch(kind){case 2:case 3:case 4:case 5:case 96:return result|1u;default:return result;}
}
uint32_t restore_building_active_004890f0(int32_t building)
{return B(0x799a0du+(uint32_t)building*264u)!=0;}
int32_t restore_building_area_00489050(int32_t x,int32_t y,int32_t columns,int32_t rows)
{
 uint32_t base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,sum=0,row,col,table,offset;
 uint16_t cw=mr_fp_control_word();MRFloat denominator=mr_fp_binary64(0),one=mr_fp_binary64(UINT64_C(0x3ff0000000000000));
 U(0xbc9768u)=base;
 if(rows>0)for(row=0;row<(uint32_t)rows;++row){
  table=0x5cc8c0u+row*48u;
  if(columns>0)for(col=0;col<(uint32_t)columns;++col){
   offset=U(table+col*8u);denominator=mr_fp_add(denominator,one,cw);
   sum+=(uint32_t)B(0xa5d4ccu+base+offset)+1u;
  }
 }
 return (int32_t)((uint32_t)truncate(mr_fp_div(mr_fp_i32((int32_t)sum),denominator,cw))+1u);
}
int32_t restore_building_quality_00489110(int32_t building)
{
 uint32_t record=0x7999d8u+(uint32_t)building*264u,score;int32_t y,x;
 y=H(record+8u);x=H(record+6u);
 score=(uint32_t)restore_building_area_00489050(x,y,3,3);
 if((uint8_t)restore_building_active_004890f0(building)){
  if((uint8_t)restore_building_flag_00488080(building)){
   if(restore_actor_building_query(building))score+=20u;else score+=40u;
  }
 }
 return (int32_t)score>99?99:(int32_t)score;
}
int32_t restore_building_capacity_00488140(int32_t building,uint8_t configured)
{
 uint32_t record=0x7999d8u+(uint32_t)building*264u;int32_t count,quality,kind,result;uint8_t flag;uint16_t cw;uint64_t spill;MRFloat value,factor;
 if(configured){kind=H(record+16u);count=(int32_t)U(0x705eb8u+(uint32_t)kind*40u);}
 else count=H(record+64u);
 quality=restore_building_quality_00489110(building);flag=B(record+169u);cw=mr_fp_control_word();
 factor=mr_fp_mul(mr_fp_binary64(UINT64_C(0x3fc47ae147ae147b)),mr_fp_binary64(UINT64_C(0x3f84afd6a052bf5b)),cw);
 value=mr_fp_mul(mr_fp_i32(quality),factor,cw);
 value=mr_fp_mul(value,mr_fp_i32(count),cw);
 value=mr_fp_mul(value,mr_fp_binary64(UINT64_C(0x4024000000000000)),cw);
 /* FST m64 always spills, but the flag-zero path retains the extended value. */
 spill=mr_fp_store64(value,cw);
 if(flag){
  if((uint8_t)FUN_00488030((uint32_t)building)){
   kind=H(record+16u);if(kind==102)goto doubled;
  }
  if((uint8_t)FUN_00488030((uint32_t)building)){
   kind=H(record+16u);if(kind==100)goto doubled;
  }
  value=mr_fp_binary64(spill);goto converted;
 doubled:
  value=mr_fp_binary64(spill);value=mr_fp_add(value,value,cw);
 }
 converted:
 result=truncate(value);return result<1?1:result;
}
uint32_t FUN_004c5340(int32_t player)
{
 uint32_t offset=(uint32_t)player*0x49d8u,id;
 if(W(0xea5ce8u+offset)){
  id=U(0xea5f9cu+offset);if(FUN_004c5120((int32_t)id)==4)return 20;
 }
 offset=U(0xeaac28u)*0x49d8u;
 if(W(0xea5ce8u+offset)){
  id=U(0xea5f9cu+offset);
  if(FUN_004c5120((int32_t)id)==0){
   offset=U(0xeaac28u)*0x49d8u;id=U(0xea5f9cu+offset);
   if(FUN_004c52d0((int32_t)id,1))return 20;
  }
 }
 return 25;
}
int32_t FUN_00447530(void)
{
 uint32_t offset=U(0xeaac28u)*0x49d8u,ratio1=0,ratio2=0,first=0,second=0,other=0,town=0,i,record,player,p,resource,price,combined;
 int32_t divisor,kind,dividend;uint16_t permission,cw;MRFloat weighted,total;
 divisor=(int32_t)U(0xea5fdcu+offset);
 if(divisor){dividend=(int32_t)U(0xea5fe0u+offset);ratio1=(uint32_t)divide(dividend,divisor);price=U(0x78d66cu);ratio1*=price;}
 divisor=(int32_t)U(0xea5fe8u+offset);
 if(divisor){dividend=(int32_t)U(0xea5fecu+offset);ratio2=(uint32_t)divide(dividend,divisor);price=U(0x78d668u);ratio2*=price;}
 for(i=1;i<4000u;++i){
  record=0x7999d8u+i*264u;
  if(B(record)!=1)continue;
  if(!(uint8_t)FUN_00488030(i)&&H(record+16u)!=194)continue;
  if((uint8_t)FUN_00488030(i)&&!(uint8_t)restore_building_flag_00489880((int32_t)i))continue;
  if(restore_actor_building_gate((int32_t)i)){
   first+=(uint32_t)restore_building_capacity_00488140((int32_t)i,1);continue;
  }
  if((uint8_t)FUN_00488030(i)){
   second+=(uint32_t)restore_building_capacity_00488140((int32_t)i,1);continue;
  }
  if(H(record+64u)<=0||H(record+84u)<=0)continue;
  kind=H(record+16u);price=U(0x705eb8u+(uint32_t)kind*40u)<<4;
  other+=(uint32_t)((int32_t)price/4);
 }
 other=(uint32_t)FUN_004cde40((int32_t)other,80);player=U(0xeaac28u);
 first=first*48u;second=second<<6;
 for(p=0x77de00u;p<0x77f742u;p+=106u){
  if(!B(p))continue;kind=B(p+2u);if(kind!=1&&kind!=3&&kind!=5)continue;
  if(!B(p+5u))continue;
  for(resource=1;resource<=8u;++resource){
   permission=W(0xea4096u+player*0x49d8u+resource*2u);
   if(permission!=1&&permission!=3)continue;
   if(B(p+42u+resource)!=1)continue;
   price=B(p+4u);price=U(0x77646cu+(price*36u+resource)*4u)*100u;
   town+=(uint32_t)((int32_t)price/12);
  }
 }
 cw=mr_fp_control_word();combined=first+second;
 weighted=mr_fp_div(mr_fp_i32((int32_t)combined),mr_fp_binary64(UINT64_C(0x4024000000000000)),cw);
 total=mr_fp_i32((int32_t)(town+other+ratio2+ratio1));total=mr_fp_add(weighted,total,cw);
 combined=(uint32_t)truncate(total);
 divisor=(int32_t)FUN_004c5340((int32_t)player);
 return divide((int32_t)(combined*100u),divisor);
}
void FUN_004e3330(int32_t tool)
{
 uint32_t offset;int32_t maximum,value;
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;if(tool!=10)return;
 offset=U(0xeaac28u)*0x49d8u;if((int32_t)U(0xea1894u+offset)<300)return;
 if(U(0x784bbcu))return;
 maximum=(int32_t)U(0xea5e4cu+offset);value=FUN_00447530();
 if(FUN_004cde60(value,maximum)>95)return;
 FUN_004e2970(94);
}
