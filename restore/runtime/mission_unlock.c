#include "mission_unlock.h"
#include "tool_availability.h"
#include "training_queries.h"
#include "tool_selector.h"
#include "event_publisher.h"
#include "sound_initialization.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_00403ef0(void)
{
 uint32_t scene=(uint32_t)(int32_t)(int8_t)B(0xe38ea8u);
 return(scene&0xffffff00u)|((scene!=U(0x57961cu))?1u:(uint32_t)(B(0x7027c5u)!=0));
}
/* Native533E70 selects ECX=785D70; no stack arguments, full EAX count. */
int32_t FUN_0044bd00(void)
{
 int32_t count=(int32_t)U(0x785d70u);uint32_t a=0x785d76u,result=0;
 if(count>0)do{if(B(a)==3)++result;a+=124u;}while(--count);
 return(int32_t)result;
}
void FUN_0053b090(int32_t sound)
{
 uint32_t sample;
 if(U(0xf55b88u)&&sound<19&&B(0xe38e63u)){
  sample=U(0xf543b8u+(uint32_t)sound*4u);
  if((int32_t)sample>=0){
   U(0xe92db0u)=U(0xe38ec4u);U(0xe92db4u)=0x40;U(0xe92dacu)=sample;
   U(0xe92da8u)=0xffffffffu;U(0xe92db8u)=1;
   FUN_0055f0e8(RESTORE_SOUND,(const int32_t *)0xe92da8u);
  }
 }
}
void FUN_004051a0(void)
{
 uint32_t i;
 for(i=0;i<0x1b0u;i+=36u){
  B(0x5c7388u+i)=1;U(0x7056ecu)=U(0x5c738cu+i);
  if(FUN_00404120()<=0)B(0x5c7388u+i)=0;
  B(0x5c75c8u+i)=1;U(0x7056ecu)=U(0x5c75ccu+i);
  if(FUN_00404120()<=0)B(0x5c75c8u+i)=0;
 }
}
void FUN_00417a70(void)
{
 uint32_t i;
 for(i=0;i<12;++i){U(0x7056ecu)=U(0x5c738cu+i*36u);U(0x6fdc58u+i*4u)=(uint32_t)FUN_00404120();}
}
void FUN_00417aa0(void)
{
 uint32_t i;
 for(i=0;i<12;++i){int32_t current;U(0x7056ecu)=U(0x5c738cu+i*36u);current=FUN_00404120();if((int32_t)U(0x6fdc58u+i*4u)<current)B(0x5c738bu+i*36u)=1;}
}
void FUN_004179f0(void)
{
 uint32_t i;int32_t scene=(int8_t)B(0xe38ea8u);
 if(scene<=10&&!U(0xe38ebcu)){
  for(i=0;i<26;++i){
   uint32_t record=0x5794bcu+i*28u;int32_t count;
   if(scene==(int32_t)U(record-12u)&&(count=(int32_t)U(record))>0){
    uint8_t enabled=B(0x7027b8u+i)!=0;uint32_t list=U(record+4u);
    do{uint32_t flag=U(list);list+=4u;B(0x6fdc88u+flag)=enabled;}while(--count);
   }
  }
 }else{
  U(0x6fdc88u)=0x01010101u;U(0x6fdc8cu)=0x01010101u;U(0x6fdc90u)=0x01010101u;W(0x6fdc94u)=0x0101;B(0x6fdc96u)=1;
 }
}
__declspec(noreturn) void restore_unlock_failure(uint32_t target){(void)target;abort();}
void restore_unlock_dispatch(uint32_t site,uint32_t target,struct restore_unlock_frame *f)
{
 uint32_t sp=f->r[4],argument=U(sp+4u);(void)site;
 switch(target){
 case 0x403ef0u:f->r[0]=FUN_00403ef0();break;
 case 0x4439c0u:f->r[0]=FUN_004439c0((int32_t)argument);break;
 case 0x443b90u:f->r[0]=FUN_00443b90((int32_t)argument);break;
 case 0x443d90u:f->r[0]=FUN_00443d90((int32_t)argument);break;
 case 0x4f5470u:f->r[0]=FUN_004f5470((int32_t)argument);break;
 default:restore_unlock_failure(target);
 }
 f->r[4]=sp+4u;
}
void FUN_00404350(void)
{
 uint32_t stack[256],i,sp;struct restore_unlock_frame f={{0},2,restore_unlock_dispatch};
 for(i=0;i<256;++i)stack[i]=0xa5a55a5au;
 sp=(uint32_t)(uintptr_t)&stack[240];f.r[4]=sp;stack[240]=0;
 restore_unlock_C_00404350(&f);
 if(f.r[4]!=sp+4u)restore_unlock_failure(0x404350u);
 for(i=0;i<16;++i)if(stack[i]!=0xa5a55a5au)restore_unlock_failure(0x404350u);
 for(i=241;i<256;++i)if(stack[i]!=0xa5a55a5au)restore_unlock_failure(0x404350u);
}
void restore_mission_unlock_body(uint8_t raw_stack[32])
{
 uint32_t i;uint8_t a,b,c;uint16_t body;uintptr_t raw=(uintptr_t)raw_stack;
 if(U(0xe38ebcu))return;
 for(i=15;i<=25;++i){
  if(!B(0x7027b8u+i)){
   int32_t scene=(int8_t)B(0xe38ea8u);
   if((int32_t)U(0x5794b0u+i*28u)==scene)FUN_00417ae0((int32_t)i);
  }
 }
 if(B(0x6fdcb4u))return;
 a=(uint8_t)(B(0x6fdcc4u)!=0&&W(0x784d2au)!=0);
 b=(uint8_t)(B(0x6fdcd0u)!=0&&W(0x784d2cu)!=0);
 c=(uint8_t)(B(0x6fdcdcu)!=0&&W(0x784d2eu)!=0);
 if(a||b||c){
  W(raw+2u)=0x248;W(raw)=0x249;
  body=a?(b?(c?0x250:0x24d):(c?0x24e:0x24a)):(b?(c?0x24f:0x24b):0x24c);
  W(raw+16u)=body;U(0xe950dcu)=1;
  FUN_004e0cf0(0x83,0,0,(const int16_t *)raw_stack,0);
 }
 B(0x6fdcb4u)=1;
}
/* Same incoming-SP minus32 scratch as 417B40. Preserve its unspecified bytes
 * as explicit raw input; only the three native WORD fields are written.
 * This promises the same transformation for the same mapped stack prestate,
 * not identical unrelated process garbage or a C uninitialized-local model. */
__declspec(naked) void FUN_00417b40(void)
{
 __asm {
  sub esp,32
  mov eax,esp
  push eax
  call restore_mission_unlock_body
  add esp,4
  add esp,32
  ret
 }
}
