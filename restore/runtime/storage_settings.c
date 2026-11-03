#include "storage_settings.h"
#include "building_controllers.h"
#include "resource_availability.h"
#include "actor_order_path.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
extern uint32_t FUN_005013a0(uint32_t resource);
uint32_t restore_storage_settings_allocate(uint32_t building)
{
 uint32_t slot,r,i,n,resource,maximum;int32_t kind;
 FUN_005596d0();
 for(slot=1;slot<200u;++slot)if(!B(0x7900c0u+slot*196u))break;
 if(slot==200u)return 0;
 r=slot*196u;FUN_0040f8f0((uint32_t *)(uintptr_t)(0x7900b8u+r),196,0);
 maximum=U(0x78d788u);B(0x7900c0u+r)=1;W(0x7900bcu+r)=(uint16_t)building;
 if((int32_t)maximum<=(int32_t)slot)U(0x78d788u)=slot;
 kind=H(0x7999e8u+building*264u);
 if(kind==70||kind==190){
  i=0;
  if((int32_t)U(0xf53e6cu)>0)do {
   resource=U(0xf53e70u+i*4u);
   if(FUN_005013a0(resource))B(0x7900c2u+r+resource)=0;
   ++i;
  }while((int32_t)i<(int32_t)U(0xf53e6cu));
  return slot;
 }
 if(kind!=71&&kind!=72)return slot;
 for(i=0;i<36u;++i){B(0x7900c2u+r+i)=1;W(0x7900ecu+r+i*2u)=3200;W(0x790134u+r+i*2u)=3200;}
 if(kind==71){
  n=U(0xf53dd8u);i=0;
  if((int32_t)n>0)do {resource=U(0xf53ddcu+i*4u);++i;B(0x7900c2u+r+resource)=0;}while(--n);
 }else{
  n=U(0xf53e6cu);i=0;
  if((int32_t)n>0)do {
   resource=U(0xf53e70u+i*4u);if((int32_t)resource<1||(int32_t)resource>8)B(0x7900c2u+r+resource)=0;
   ++i;
  }while(--n);
 }
 return slot;
}
void restore_building_control_C_0046b100(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=restore_storage_settings_allocate(U(sp+4u));f->r[4]=sp+4u;}
