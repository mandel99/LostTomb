#include "monument_base_queries.h"
#include "campaign_score_numeric.h"
#define MB_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MB_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MB_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern __declspec(noreturn) void restore_monument_progress_invalid(uint32_t token);
void restore_monument_base_initialize(void)
{MB_U(0x6fd5dcu)=2;MB_U(0x6fd36cu)=2;MB_U(0x6de54cu)=3;}
uint8_t restore_monument_base_worker(uint32_t token,uint32_t building)
{
 uint32_t slot,worker,actor,state;uint8_t type;
 if(token!=0x40ec20u&&token!=0x417590u&&token!=0x4e6b20u)restore_monument_progress_invalid(token);
 if(token==0x40ec20u&&(int32_t)MB_U(0x799a30u+building*264u)>=3)building-=(uint32_t)(int32_t)(int16_t)MB_W(0x7999eau+building*264u);
 slot=0x799a34u+building*264u;worker=MB_U(slot);if(!worker)return 0;
 actor=worker*388u;
 if(!MB_B(0xbcd606u+actor)){MB_U(slot)=0;return 0;}
 state=MB_B(0xbcd630u+actor);if(state==6||state==7)state=MB_B(0xbcd608u+actor);
 type=MB_B(0xbcd602u+actor);
 if(type==0x4f)return state==9||state==10;
 if(type==0x51)return state==9||state==11||state==10;
 if(type==0x55){
  if(state==15||state==16)return 1;
  if(token==0x40ec20u)return state==31||state==32||state==33;
  return state==(token==0x417590u?34u:35u);
 }
 MB_U(slot)=0;return 0;
}
static int base_actor(void *self,uint32_t root,uint32_t *q,uint32_t *q10,uint32_t *u,uint32_t *o)
{
 uint32_t table=MB_U((uint32_t)(uintptr_t)self),token,worker,actor;uint8_t type;
 if(table!=0x56f48cu&&table!=0x56f4dcu&&table!=0x56f6e8u)restore_monument_progress_invalid(table);
 token=MB_U(table+0x24u);
 if(restore_monument_base_worker(token,root)){
  worker=MB_U(0x799a34u+root*264u);if(!worker)return 0;
  actor=worker*388u;type=MB_B(0xbcd602u+actor);
  if(type==0x4f)*o=1;
  else if(type==0x51){uint8_t state=MB_B(0xbcd630u+actor);*q=1;if(state==10)*q10=1;}
  else if(type==0x55)*u=1;
 }
 return 1;
}
static uint32_t base_root(uint32_t building)
{return building-(uint32_t)(int32_t)(int16_t)MB_W(0x7999eau+building*264u);}
static uint32_t positive_cost(uint32_t value)
{return(value&0x80000000u)?0u:value;}
int32_t restore_monument_base_0040d940(void *self,uint32_t building,int32_t *output)
{
 uint32_t root=base_root(building),stage=MB_U(0x799a30u+root*264u),percent,cost=0x3840,q=0,q10=0,u=0,o=0;
 if(stage==21)percent=100;
 else{
  uint8_t raw80[10];
  __asm {
   fild stage
   mov eax,056f4c8h
   fmul qword ptr[eax]
   mov eax,056f470h
   fmul qword ptr[eax]
   lea eax,raw80
   fstp tbyte ptr[eax]
  }
  percent=(uint32_t)restore_campaign_score_trunc80(raw80);
 }
 if(!base_actor(self,root,&q,&q10,&u,&o))return 0;
 if((int32_t)stage>3)cost=positive_cost((36u-(stage-3u)*MB_U((uint32_t)(uintptr_t)self+4u))*400u);
 if(output){volatile uint32_t *out=(volatile uint32_t *)output;out[11]=cost;out[8]=q10;out[9]=u;out[7]=q;out[0]=stage;out[10]=o;out[12]=percent;out[13]=0x3840;}
 return(int32_t)percent;
}
int32_t restore_monument_base_00417050(void *self,uint32_t building,int32_t *output)
{
 uint32_t root=base_root(building),stage=MB_U(0x799a30u+root*264u),percent=stage==19?100u:stage*5u+5u,cost,q=0,q10=0,u=0,o=0;
 if(!base_actor(self,root,&q,&q10,&u,&o))return 0;
 cost=positive_cost((38u-MB_U((uint32_t)(uintptr_t)self+4u)*stage)*400u);
 if(output){volatile uint32_t *out=(volatile uint32_t *)output;out[0]=stage;out[2]=q10;out[3]=u;out[5]=cost;out[1]=q;out[4]=o;out[6]=percent;out[7]=0x3b60;}
 return(int32_t)percent;
}
int32_t restore_monument_base_004e6710(void *self,uint32_t building,int32_t *output)
{
 uint32_t root=base_root(building),stage=MB_U(0x799a30u+root*264u),percent=stage==9?100u:(stage*5u+5u)*2u,cost,q=0,q10=0,u=0,o=0;
 if(!base_actor(self,root,&q,&q10,&u,&o))return 0;
 cost=positive_cost((9u-stage)*MB_U((uint32_t)(uintptr_t)self+4u)*400u);
 if(output){volatile uint32_t *out=(volatile uint32_t *)output;out[0]=stage;out[1]=q;out[2]=q10;out[3]=u;out[4]=o;out[5]=cost;out[6]=percent;out[7]=MB_U((uint32_t)(uintptr_t)self+4u)*3600u;}
 return(int32_t)percent;
}
