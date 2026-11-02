#include "actor_work.h"
#include "audio_state.h"
#include "animation_state.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0053d820(void);

/* 41FD80 is a task-object member, not an actor-only cleanup routine. */
void FUN_0041fd80(void *record,int32_t actor)
{
 uintptr_t object=(uintptr_t)record;
 H(object+18u)=(int16_t)actor;
 if(actor)B(object+20u)=B(0xbcd630u+(uint32_t)actor*388u);
}

/* 4200B0: a job remains while any matching task has positive work. */
void FUN_004200b0(void)
{
 uint32_t entry,job=(uint32_t)(int32_t)H(0xea5baau+U(0xeaac28u)*0x49d8u);
 for(entry=0x5c2748u;entry<0x5c28a8u;entry+=16u)
  if(U(entry)==job&&(int32_t)U(U(entry+8u)+12u)>0)return;
 FUN_0053d820();
}

/* 420130: native first argument is unused. The22 descriptors contain
 * a job DWORD, an actor-kind DWORD and an object pointer, each16 bytes apart. */
void FUN_00420130(uint32_t unused,int32_t actor,uint8_t next_state)
{
 uint32_t entry,object,offset=(uint32_t)actor*388u;
 uint32_t player=U(0xeaac28u),job;
 (void)unused;
 B(0xbcd65du+offset)=1;
 for(entry=0x5c2748u;entry<0x5c28a8u;entry+=16u){
  object=U(entry+8u);
  if((int32_t)H(object+18u)!=actor)continue;
  job=(uint32_t)(int32_t)H(0xea5baau+player*0x49d8u);
  if(U(entry)!=job)goto complete;
  goto perform;
 }
 for(entry=0x5c2748u;entry<0x5c28a8u;entry+=16u){
  object=U(entry+8u);
  if(H(object+18u))continue;
  job=(uint32_t)(int32_t)H(0xea5baau+player*0x49d8u);
  if(U(entry)==job&&U(entry+4u)==B(0xbcd602u+offset)&&
     (int32_t)U(object+12u)>0)goto perform;
 }
 goto complete;
perform:
 object=U(entry+8u);
 if(!H(object+18u)){
  FUN_0041fd80((void *)(uintptr_t)object,actor);
  FUN_0041fca0((void *)(uintptr_t)U(entry+8u),0);
 }
 object=U(entry+8u);U(object+12u)-=1u;
 object=U(entry+8u);
 if((int32_t)U(object+12u)>=1)return;
 FUN_004200b0();
complete:
 B(0xbcd630u+offset)=next_state;
}

/* 420350: find free work first, then publish the first matching destination. */
int32_t FUN_00420350(uint32_t unused1,uint32_t unused2,int32_t actor)
{
 uint32_t entry,object,building,offset=(uint32_t)actor*388u;
 uint32_t job=(uint32_t)(int32_t)H(0xea5baau+U(0xeaac28u)*0x49d8u);
 (void)unused1;(void)unused2;
 for(entry=0x5c2748u;entry<0x5c28a8u;entry+=16u){
  object=U(entry+8u);
  if(!H(object+18u)&&U(entry)==job&&
     U(entry+4u)==B(0xbcd602u+offset)&&(int32_t)U(object+12u)>0)break;
 }
 if(entry==0x5c28a8u)return 0;
 for(building=1;building<4000u;++building){
  uint32_t source=building*264u;
  if(B(0x7999d8u+source)!=1||H(0x7999e8u+source)!=210)continue;
  H(0xbcd61cu+offset)=H(0x7999feu+source);
  H(0xbcd61eu+offset)=H(0x799a00u+source);
  U(0xbcd620u+offset)=U(0x7999e4u+source);
  H(0xbcd63cu+offset)=0;
  return (int32_t)building;
 }
 return 0;
}

/* 420C40: stage belongs to the root selected by a signed building-link WORD. */
int32_t FUN_00420c40(int32_t building,int32_t actor)
{
 uint32_t entry,object,offset=(uint32_t)actor*388u;
 uint32_t root=(uint32_t)building-(uint32_t)(int32_t)H(0x7999eau+(uint32_t)building*264u);
 uint32_t stage=U(0x799a2cu+root*264u);
 for(entry=0x5a8140u;entry<0x5a84c0u;entry+=16u){
  object=U(entry+8u);
  if(!H(object+18u)&&U(entry)==stage&&
     U(entry+4u)==B(0xbcd602u+offset)&&(int32_t)U(object+12u)>0)return building;
 }
 return 0;
}

/* 4E5A30: preserve the WORD kind and signed, truncating progress division. */
int32_t FUN_004e5a30(int32_t building)
{
 uint32_t offset=(uint32_t)building*264u;
 int32_t count=H(0x7999e8u+offset)==207?10:18;
 int32_t stage=((int32_t)B(0x799a2du+offset)-33)*count/66+1;
 return stage<count?stage:count;
}
