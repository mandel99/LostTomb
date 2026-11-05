#include "city_monument_completion.h"
#include "menu_building_overlay.h"
#include "campaign_score_numeric.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "monument_progress.h"
#include "remaining_selection_actions.h"
#include "remaining_menu_actions.h"
#include "event_publisher.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SELF(a) ((void *)(uintptr_t)(a))

/* 46F200: scan native building IDs, publish at most one completion message.
   Query results are signed DWORDs; announcement fields are individual BYTEs.
   The scenario latch and global message latch are DWORDs. All index arithmetic
   wraps at 32 bits, including skips supplied by the live shape tables. */
void restore_city_publish_completed_monument(void)
{
 uint32_t building=1,record,family,kind,scene,flag,skip,code;
 int32_t progress;
 int16_t parameters[3]={-1,-1,-1};
 uint8_t reset;
 do{
  record=0x7999d8u+building*264u;
  if(B(record)==1u){
   flag=0;skip=0;code=0;reset=0;progress=0;
   switch((int32_t)(int16_t)W(record+16u)){
   case 0xb7:
    if(FUN_004f6020(building)&&B(record+128u)==0){
     family=U(0x5e4ef4u);code=0xffffu;
     switch(family){
     case 1:code=0x22au;break;
     case 2:code=0x22cu;break;
     case 3:code=0x228u;break;
     case 4:code=0x226u;break;
     case 5:code=0x22eu;break;
     }
     B(0x799a58u+building*264u)=1;
     goto publish;
    }
    family=U(0x5e4ef4u);kind=B(record+84u);
    building+=U((family==5u?0x56f7ecu:0x56f7d8u)+kind*4u)-1u;
    break;
   case 0xcf:case 0xd0:
    if(B(record+85u)==100u&&B(record+128u)==0){
     code=0x232u;B(0x799a58u+building*264u)=1;goto publish;
    }
    break;
   case 0xd2:
    scene=U(0xeaac28u)*0x49d8u;
    if((int16_t)W(0xea5baau+scene)>=16&&U(0xea41f8u+scene)==0){
     code=0x230u;U(0xea41f8u+scene)=1;goto publish;
    }
    break;
   case 0xd5:
    progress=FUN_00542ad0(building,0);flag=104;skip=99;code=0x234;break;
   case 0xde:
    progress=FUN_004df040(building,0);flag=116;skip=0x73;code=0x23e;break;
   case 0xe1:
    progress=restore_monument_base_0040d940(SELF(0x6fd5d8u),building,0);
    flag=104;skip=0x18;code=0x236;break;
   case 0xe3:
    progress=restore_monument_base_00417050(SELF(0x6fd368u),building,0);
    flag=104;skip=8;code=0x23a;break;
   case 0xe4:
    progress=restore_monument_base_004e6710(SELF(0x6de548u),building,0);
    flag=104;skip=1;code=0x23c;break;
   case 0xe5:
    progress=restore_monument_progress_00545740(SELF(0x6f9320u),building,0);
    flag=157;skip=0xa6;code=0x240;reset=1;
    /* Native loads the signed root link only after progress reaches100. */
    if(progress>=100&&(int16_t)W(record+22u)<-1)progress=0;
    break;
   case 0xe6:
    /* 401540's existing complete C does not consume the native ECX value. */
    progress=FUN_00401540(building,0);flag=96;skip=0xf;code=0x238;break;
   case 0xea:
    progress=restore_monument_progress_00558be0(SELF(0x6f5648u),building,0);
    flag=157;skip=0x9d;code=0x242;reset=1;break;
   case 0xeb:
    progress=restore_monument_progress_00555e80(SELF(0x6de9e0u),building,0);
    flag=157;skip=0x1b4;code=0x244;reset=1;break;
   case 0xec:
    progress=restore_monument_progress_0054f470(SELF(0x6e9188u),building,0);
    flag=157;skip=0x200;code=0x246;reset=1;break;
   }
   if(flag){
    if(progress>=100&&B(record+flag)==0){
     if(reset){restore_remaining_selection_00547a60();FUN_004dea50();}
     B(0x7999d8u+building*264u+flag)=1;
     goto publish;
    }
    building+=skip;
   }
  }
  ++building;
 }while((int32_t)building<4000);
 return;
publish:
 if(code!=0xffffu){parameters[0]=(int16_t)(code+1u);parameters[1]=(int16_t)code;}
 U(0xe950dcu)=1;
 FUN_004e0cf0(0x92,0xffffffffu,0,parameters,0);
}
