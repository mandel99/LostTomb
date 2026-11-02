#include "actor_teardown_model.h"
#include "actor_walking.h"
#include "menu_building_kind.h"
#include "menu_actor_message.h"
#include "host_allocator.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
/* Full accepted 4894A0 -> 489470 -> 4893C0 C bodies, native RET4/RET16 remain inside this frame. */
uint8_t restore_actor_building_query(int32_t building)
{
 uint32_t stack[32];struct restore_actor_message_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=restore_actor_message_dispatch;f.r[4]=(uint32_t)(uintptr_t)(stack+16);stack[17]=(uint32_t)building;
 restore_actor_message_C_004894a0(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+18)||stack[16]!=SENTRY||stack[0]!=SENTRY)abort();
 for(i=18;i<32;++i)if(stack[i]!=SENTRY)abort();
 return (uint8_t)f.r[0];
}
/* ECX receiver of original488050 is only forwarded to4894A0, whose complete closure does not read it. */
uint32_t restore_actor_building_gate(int32_t building)
{
 if((uint8_t)FUN_00488030((uint32_t)building)==0)return 0;
 return restore_actor_building_query(building)!=0;
}
/* Native 4F6A50 grows through 4F66C0/6700/6790/66F0. Its final copy reads
   the destination capacity from a smaller temporary allocation and loses
   wrapped FIFO entries. Preserve the logical entries without that overread. */
uint32_t restore_ring_004f6a50(void *self,int32_t capacity)
{
 uint32_t q=(uint32_t)(uintptr_t)self,old_capacity=U(q+16u),count,head,i,tail;
 uint32_t old_buffer,new_buffer;size_t bytes;
 if(capacity<=(int32_t)old_capacity)return 1;
 if(capacity<=0||(uint32_t)capacity>UINT32_MAX/4u)abort();
 count=U(q+12u);head=U(q+4u);old_buffer=U(q+20u);
 if(count>old_capacity||(count&&(!old_capacity||!old_buffer)))abort();
 bytes=(size_t)(uint32_t)capacity*4u;
 new_buffer=(uint32_t)(uintptr_t)restore_heap_allocate(bytes);
 if(!new_buffer)abort();
 memset((void *)(uintptr_t)new_buffer,0,bytes);
 head%=(uint32_t)capacity;
 for(i=0;i<count;++i){
  uint32_t source=(head+i)%old_capacity,destination=(head+i)%(uint32_t)capacity;
  U(new_buffer+destination*4u)=U(old_buffer+source*4u);
 }
 tail=(head+count+(uint32_t)capacity-1u)%(uint32_t)capacity;
 U(q+16u)=(uint32_t)capacity;U(q+20u)=new_buffer;
 U(q+4u)=head;U(q+8u)=tail;
 if(old_buffer)restore_heap_free((void *)(uintptr_t)old_buffer);
 return 1;
}
uint8_t restore_ring_004f6950(void *self,int32_t actor)
{
 uint32_t q=(uint32_t)(uintptr_t)self,count,scanned=0,buffer,value,tail;
 volatile uint32_t index=U(q+4u),next;
 count=U(q+12u);
 if((int32_t)count>0){
  do {
   buffer=U(q+20u);
   if(U(buffer+index*4u)==(uint32_t)actor)break;
   index=index+1u;(void)restore_ring_004f6a10(self,&index);
   count=U(q+12u);scanned++;
  }while((int32_t)scanned<(int32_t)count);
 }
 count=U(q+12u);
 if(scanned==count)return 0;
 if((int32_t)scanned<(int32_t)(count-1u)){
  do {
   next=index+1u;(void)restore_ring_004f6a10(self,&next);
   buffer=U(q+20u);value=U(buffer+next*4u);U(buffer+index*4u)=value;
   index=index+1u;(void)restore_ring_004f6a10(self,&index);
   count=U(q+12u);scanned++;
  }while((int32_t)scanned<(int32_t)(count-1u));
 }
 tail=U(q+8u);count=U(q+12u);U(q+8u)=tail-1u;U(q+12u)=count-1u;
 (void)restore_ring_004f6a10(self,(volatile uint32_t *)(uintptr_t)(q+8u));
 return 1;
}
uint32_t FUN_004f68b0(void *self)
{
 uint32_t q=(uint32_t)(uintptr_t)self,head;
 if(!U(q+12u))return 0;
 head=U(q+4u);U(q+4u)=head+1u;
 (void)restore_ring_004f6a10(self,(volatile uint32_t *)(uintptr_t)(q+4u));
 U(q+12u)=U(q+12u)-1u;
 return U(U(q+20u)+head*4u);
}
/* Native 4F68A0 is a jump alias; 4F68F0 forwards to the push member. */
uint32_t FUN_004f68a0(void *self){return FUN_004f68b0(self);}
void FUN_004f6900(void *self,uint32_t value)
{
 uint32_t q=(uint32_t)(uintptr_t)self;
 if(U(q+12u)==U(q+16u)&&
    !restore_ring_004f6a50(self,(int32_t)(U(q+16u)+10u)))return;
 if(!U(q+20u))return;
 U(q+8u)=U(q+8u)+1u;
 (void)restore_ring_004f6a10(self,(volatile uint32_t *)(uintptr_t)(q+8u));
 U(U(q+20u)+U(q+8u)*4u)=value;
 U(q+12u)=U(q+12u)+1u;
}
void FUN_004f68f0(void *self,uint32_t value){FUN_004f6900(self,value);}
uint8_t FUN_004f6950(void *self,int32_t value)
{return restore_ring_004f6950(self,value);}
uint8_t FUN_004f6a10(void *self,volatile uint32_t *index)
{return restore_ring_004f6a10(self,index);}
uint8_t FUN_004a43a0(int32_t actor)
{
 uint32_t building=(uint32_t)(int32_t)(int16_t)W(0xbcd656u+(uint32_t)actor*388u);
 uint8_t list=B(0x799aa3u+building*264u);
 if(list==0||list>=10)return 0;
 (void)restore_ring_004f6950((void *)(uintptr_t)(0x9783a4u+(uint32_t)list*28u),actor);
 return 1;
}
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4],actor=U(sp+4u);(void)site;
 switch(target){
 case 0x488050u:f->r[0]=restore_actor_building_gate((int32_t)actor);f->r[4]=sp+8u;return;
 case 0x4a43a0u:f->r[0]=(f->r[0]&UINT32_C(0xffffff00))|FUN_004a43a0((int32_t)actor);break;
 case 0x4b3250u:FUN_004b3250((int32_t)actor);break;
 case 0x4b5420u:FUN_004b5420((int32_t)actor);break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
void FUN_004b4980(int32_t actor)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+16);stack[17]=(uint32_t)actor;
 restore_C_004b4980(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<9;++i)if(stack[i]!=SENTRY)abort();
 for(i=18;i<32;++i)if(stack[i]!=SENTRY)abort();
}

