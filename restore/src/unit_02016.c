#include "random_building_patrol.h"
#include "actor_callbacks_human.h"
#include "actor_create.h"
#include "../artifacts/actor_civic_followup/actor_civic_followup.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

/* 4E7AC0: two live building scans surrounding the native31-step generator.
   Counts and latches are DWORDs. Preserve the physical EAX result even on
   early exits; the query's logical Boolean is its low BYTE only. */
uint32_t restore_spawn_patrol_at_random_building(int32_t duration)
{
 uint32_t eax,building=1,eligible=0,limit,seen=0,selected;
 uint32_t ring,left,right,i,offset,actor,player;
 int32_t x,y;
 U(0x78d798u)=(uint32_t)duration;
 eax=U(0x78d790u);
 if((int32_t)eax<1)return eax;
 do{
  eax=restore_building_patrol_eligibility_result((int32_t)building);
  if((uint8_t)eax)++eligible;
  limit=U(0x78d790u);
  ++building;
 }while((int32_t)building<=(int32_t)limit);
 if((int32_t)eligible<=0)return eax;

 ring=U(0xd3b1c4u);eax=U(0xd3b374u);
 U(0xd3b1c8u+ring*4u)=eax;
 ++ring;U(0xd3b1c4u)=ring;
 if((int32_t)ring>=100)U(0xd3b1c4u)=0;
 left=U(0xd3b384u);right=U(0xd3b380u);
 for(i=0;i<31u;++i){
  left=(left>>1)|((((left>>4)^left)&1u)<<30);
  right=(right>>1)|((((right>>4)^right)&1u)<<30);
 }
 /* The two seeds are published at different native phases. Keep the store
    order; the other generator users have different publication protocols. */
 U(0xd3b384u)=left;
 U(0xd3b378u)=left&7u;
 U(0xd3b364u)=right&0x7fffu;
 U(0xd3b370u)=left&0x7fffu;
 U(0xd3b368u)=right&0x7fu;
 eax=(left&0x7fffu)/eligible;
 selected=(left&0x7fffu)%eligible+1u;
 U(0xd3b374u)=left&0x7fu;
 U(0xd3b380u)=right;
 U(0xd3b36cu)=right&7u;
 if((int32_t)limit<1)return eax;

 building=1;
 for(;;){
  eax=restore_building_patrol_eligibility_result((int32_t)building);
  if((uint8_t)eax){
   ++seen;
   if(seen==selected)break;
  }
  eax=U(0x78d790u);
  ++building;
  if((int32_t)building>(int32_t)eax)return eax;
 }
 offset=building*264u;
 y=H(0x799a00u+offset);x=H(0x7999feu+offset);
 (void)FUN_0047fe60(x,y,1,6);
 y=(int32_t)U(0xd3b4e0u);x=(int32_t)U(0xd3b4e4u);
 player=U(0xeaac28u);
 actor=(uint32_t)FUN_004b4800((uint8_t)player,0x6d,x,y,0);
 offset=actor*388u;
 W(0xbcd652u+offset)=(uint16_t)building;
 B(0xbcd630u+offset)=9;
 W(0xbcd62eu+offset)=0;
 /* Reuse the already reconstructed full-EAX4B1DF0 body, without sampling
    its void legacy facade or copying its patrol algorithm. */
 eax=restore_actor_prepare_service_patrol((int32_t)actor);
 W(0xbcd63cu+offset)=0;
 return eax;
}
