#include "resource_availability.h"
#include "training_queries.h"
#include <stdio.h>
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))
/* The native Boolean contract is AL. Retain its full legacy EAX as well. */
static uint32_t yes(uint32_t value){return (value&0xffffff00u)|1u;}
static uint32_t no(uint32_t value){return value&0xffffff00u;}
uint32_t restore_resource_event_supply(int32_t resource)
{
 uint32_t count=U(0x785d70u),index=count-1u,record=0x785d74u+index*124u,value;
 if(count)do {
  if(I(index)>=0&&I(index)<I(count)&&record&&B(record+2u)==23u){
   if(H(record+6u)==resource||H(record+8u)==resource||H(record+10u)==resource)return yes(index);
  }
  record-=124u;
 }while(index--);
 if(resource==12){
  value=FUN_00443b90(11);if(!value)value=FUN_00443d90(11);
  if(value){value=restore_resource_event_supply(9);if((uint8_t)value)return yes(value);}
  value=restore_resource_event_supply(11);
  if((uint8_t)value){value=FUN_00443b90(1);if(value)return yes(value);}
  value=FUN_004439c0(11);
  if(value){value=restore_resource_event_supply(9);if((uint8_t)value)return yes(value);}
  value=restore_resource_event_supply(11);
  if((uint8_t)value){value=FUN_004439c0(9);if(value)return yes(value);}
  value=restore_resource_event_supply(11);if(!(uint8_t)value)return no(value);
  value=restore_resource_event_supply(9);return (uint8_t)value?yes(value):no(value);
 }
 if(resource==34){
  value=FUN_00443b90(11);
  if(!value){
   value=FUN_00443d90(11);
   if(!value){
    value=restore_resource_event_supply(11);
    if(!(uint8_t)value){value=restore_resource_event_supply(13);if(!(uint8_t)value)return no(value);}
   }
  }
  value=FUN_004439c0(31);return value?yes(value):no(value);
 }
 value=(uint32_t)resource-10u;
 switch(resource){
 case 10:if(!W(0x784d32u))return no(value);resource=29;break;
 case 13:resource=11;break;
 case 15:resource=14;break;
 case 17:resource=16;break;
 case 19:resource=18;break;
 case 23:resource=22;break;
 case 28:if(!W(0x784d34u))return no(value);resource=20;break;
 case 33:resource=32;break;
 default:return no(value);
 }
 value=restore_resource_event_supply(resource);return (uint8_t)value?yes(value):no(value);
}
void restore_resource_lists_rebuild(void)
{
 uint32_t resource,index,count,player;
 U(0xf53e6cu)=0;
 for(index=0;index<36u;++index)U(0xf53e70u+index*4u)=0;
 for(index=0;index<36u;++index)U(0xf53ddcu+index*4u)=0;
 U(0xf53dd8u)=0;
 for(resource=1;resource<36u;++resource){
  if(FUN_00443b90((int32_t)resource)||FUN_004439c0((int32_t)resource)||
     (uint8_t)restore_resource_event_supply((int32_t)resource)||B(0xea4205u+U(0xeaac28u)*0x49d8u+resource)){
   index=U(0xf53e6cu);U(0xf53e70u+index*4u)=resource;U(0xf53e6cu)=index+1u;
  }
 }
 /* The native nonpositive-index recovery block is unreachable: this private
    index starts at1, advances to8 and cannot be changed by its callees. */
 for(resource=1;resource<=8u;++resource){
  if(FUN_00443b90((int32_t)resource)||FUN_004439c0((int32_t)resource)||
     (uint8_t)restore_resource_event_supply((int32_t)resource)||B(0xea41fcu+U(0xeaac28u)*0x49d8u+resource)){
   index=U(0xf53dd8u);U(0xf53ddcu+index*4u)=resource;U(0xf53dd8u)=index+1u;
  }
 }
 count=U(0xf53e6cu);player=U(0xeaac28u)*0x49d8u;
 if(I(count)>0)for(index=0;index<count;++index){resource=U(0xf53e70u+index*4u);if(resource)B(0xea4205u+player+resource)=1;}
 count=U(0xf53dd8u);
 if(I(count)>0)for(index=0;index<count;++index){resource=U(0xf53ddcu+index*4u);if(resource)B(0xea41fcu+player+resource)=1;}
}
void restore_avail_C_00559480(struct restore_resource_availability_frame *f)
{uint32_t sp=f->r[4];f->r[0]=restore_resource_event_supply(I(U(sp+4u)));f->r[4]=sp+4u;}
void restore_avail_C_005596d0(struct restore_resource_availability_frame *f)
{restore_resource_lists_rebuild();f->r[4]+=4u;}
__declspec(noinline) __declspec(noreturn) void restore_resource_availability_failure(uint32_t target)
{fprintf(stderr,"Unsupported resource availability target %08lx\n",(unsigned long)target);abort();}
void restore_resource_availability_dispatch(uint32_t site,uint32_t target,struct restore_resource_availability_frame *f)
{
 uint32_t sp=f->r[4],argument;(void)site;
 if(target==0x559480u){restore_avail_C_00559480(f);return;}
 if(target==0x5596d0u){restore_avail_C_005596d0(f);return;}
 argument=U(sp+4u);
 switch(target){
 case 0x4439c0u:f->r[0]=FUN_004439c0(I(argument));break;
 case 0x443b90u:f->r[0]=FUN_00443b90(I(argument));break;
 case 0x443d90u:f->r[0]=FUN_00443d90(I(argument));break;
 default:restore_resource_availability_failure(target);
 }
 f->r[4]=sp+4u;
}
