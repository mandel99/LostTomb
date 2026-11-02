#include "monument_progress.h"
#include "campaign_score_numeric.h"
#include <stdio.h>
#include <stdlib.h>
#define MP_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MP_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MP_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

__declspec(noinline) __declspec(noreturn) void restore_monument_progress_invalid(uint32_t token)
{fprintf(stderr,"Unsupported monument progress callback %08lx\n",(unsigned long)token);abort();}
uint8_t restore_monument_worker_active(uint32_t building)
{
 uint32_t slot=0x799a34u+building*264u,worker=MP_U(slot),actor,state;uint8_t type;
 if(!worker)return 0;
 actor=worker*388u;
 if(!MP_B(0xbcd606u+actor)){MP_U(slot)=0;return 0;}
 state=MP_B(0xbcd630u+actor);
 if(state==6||state==7)state=MP_B(0xbcd608u+actor);
 type=MP_B(0xbcd602u+actor);
 if(type==0x51)return state==9||state==11;
 if(type==0x6c)return state==9||state==12;
 MP_U(slot)=0;return 0;
}
static uint8_t progress_worker(void *self,uint32_t building)
{
 uint32_t table=MP_U((uint32_t)(uintptr_t)self),token;
 if(table!=0x570160u&&table!=0x570268u&&table!=0x570228u&&table!=0x5701e8u)restore_monument_progress_invalid(table);
 token=MP_U(table+0x24u);
 if(token!=0x5477e0u)restore_monument_progress_invalid(token);
 return restore_monument_worker_active(building);
}
__declspec(noinline) int32_t restore_monument_progress_percent(int32_t sum,int32_t count)
{
 uint8_t raw80[10];uint16_t status;
 __asm {
  fild count
  mov eax,056f550h
  fmul dword ptr[eax]
  mov eax,056f6dch
  fdivr dword ptr[eax]
  fimul sum
  mov eax,05701e0h
  fcom qword ptr[eax]
  fnstsw status
 }
 if(!(status&0x4100u)){
  __asm {fstp st(0)}
  return 100;
 }
 __asm {
  lea eax,raw80
  fstp tbyte ptr[eax]
 }
 return(int32_t)(uint32_t)restore_campaign_score_trunc80(raw80);
}
int32_t restore_monument_progress(void *self,uint32_t building,int32_t count,int32_t *output)
{
 uint32_t root=building-(uint32_t)(int32_t)(int16_t)MP_W(0x7999eau+building*264u),end=root+(uint32_t)count;
 uint32_t sum=0,q=0,u=0,l=0,i,slot;int32_t percent;
 if((int32_t)root<(int32_t)end){
  uint32_t n=end-root;slot=0x799a38u+root*264u;
  do{sum+=MP_U(slot);slot+=264u;}while(--n);
 }
 percent=restore_monument_progress_percent((int32_t)sum,count);
 slot=0x799a34u+root*264u;
 for(i=0;(int32_t)i<count;++i,slot+=264u){
  if(progress_worker(self,root)){
   uint32_t worker=MP_U(slot);uint8_t type;
   if(!worker)break;
   type=MP_B(0xbcd602u+worker*388u);
   if(type==0x51)++q;else if(type==0x55)++u;else if(type==0x6c)++l;
  }
 }
 if(output){
  volatile int32_t *out=output;
  out[5]=percent;out[1]=(int32_t)q;out[2]=(int32_t)u;out[4]=(int32_t)l;out[6]=(int32_t)MP_U(0x799a70u+root*264u);
 }
 return percent;
}
int32_t restore_monument_progress_00545740(void *self,uint32_t building,int32_t *output)
{return restore_monument_progress(self,building,9,output);}
int32_t restore_monument_progress_00558be0(void *self,uint32_t building,int32_t *output)
{return restore_monument_progress(self,building,10,output);}
int32_t restore_monument_progress_00555e80(void *self,uint32_t building,int32_t *output)
{return restore_monument_progress(self,building,21,output);}
int32_t restore_monument_progress_0054f470(void *self,uint32_t building,int32_t *output)
{return restore_monument_progress(self,building,31,output);}
