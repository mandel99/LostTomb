#include "actor_create.h"
#include "actor_ring_queue.h"
#include "actor_walking.h"
#include "actor_profile_model.h"
extern uint32_t FUN_00562348(void);
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Original queue receiver is ECX and RET4 removes the actor slot. */
uint32_t restore_ring_004f6570(const void *queue,int32_t actor)
{
 return restore_actor_ring_contains(queue,(uint32_t)actor);
}
/* Only AL is defined by the original boolean return. */
__declspec(noinline) uint8_t FUN_004b47a0(int32_t actor)
{
 uint32_t q,record,j;
 for(q=0xeaaf68u;q<0xeadb28u;q+=224u)
  if(U(q)!=0 && restore_ring_004f6570((const void *)(uintptr_t)q,actor)!=0)return 1;
 for(record=0x9784dcu;record<0x978d74u;record+=44u)
  if(B(record-4u)!=0)
   for(j=0;j<4u;++j)if(U(record+j*4u)==(uint32_t)actor)return 1;
 return 0;
}
__declspec(noinline) void FUN_00444b30(int32_t actor)
{
 uint32_t slot=U(0x77aefcu),next;
 B(0xbcd677u+(uint32_t)actor*388u)=(uint8_t)slot;
 FUN_0040f8f0((uint32_t *)(uintptr_t)(0x77af00u+slot*88u),88,0);
 next=U(0x77aefcu)+1u;U(0x77aefcu)=next;
 if((int32_t)next>=100)U(0x77aefcu)=0;
}
__declspec(noinline) int32_t restore_actor_create(uint8_t player,int32_t kind,int32_t x,int32_t y,uint8_t direction)
{
 uint32_t actor,record,j,serial,tile;uint8_t random;
 for(actor=1;actor<2000u;++actor)
  if(B(0xbcd606u+actor*388u)==0 && FUN_004b47a0((int32_t)actor)==0){
   record=0xbcd5f8u+actor*388u;
   for(j=0;j<97u;++j)U(record+j*4u)=0;
   B(record+14u)=1;B(record+15u)=player;
   serial=U(0xd056fcu);B(record+10u)=(uint8_t)kind;B(record+12u)=0;B(record+13u)=1;
   W(record+306u)=UINT16_MAX;W(record+134u)=(uint16_t)serial;B(record+17u)=direction;
   W(record+44u)=(uint16_t)x;U(0xd056fcu)=serial+1u;
   W(record+36u)=(uint16_t)x;W(record+24u)=(uint16_t)x;W(record+20u)=(uint16_t)x;
   W(record+46u)=(uint16_t)y;W(record+38u)=(uint16_t)y;W(record+26u)=(uint16_t)y;W(record+22u)=(uint16_t)y;
   tile=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;U(record+32u)=tile;
   W(record+74u)=(uint16_t)(W(record+20u)*15u);W(record+76u)=(uint16_t)(W(record+22u)*15u);
   B(record+57u)=15;B(record+146u)=0;
   random=(uint8_t)(FUN_00562348()&3u);B(record+126u)=random;B(record+124u)=random;
   FUN_004b5af0((int32_t)actor);FUN_004b52f0((int32_t)actor);
   if(kind==19||kind==20)FUN_00444b30((int32_t)actor);
   if((int32_t)actor>(int32_t)U(0xbcd5f0u))U(0xbcd5f0u)=actor;
   return (int32_t)actor;
  }
 return 0;
}
__declspec(noinline) int32_t FUN_004b4800(uint8_t player,int32_t kind,int32_t x,int32_t y,uint8_t direction)
{return restore_actor_create(player,kind,x,y,direction);}
