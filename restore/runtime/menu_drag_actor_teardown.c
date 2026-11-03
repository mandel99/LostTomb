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
#define H(a) ((int32_t)(int16_t)W(a))
static int actor_releases_reservation(int32_t kind)
{
 if(kind>=110&&kind<=114)return 1;
 switch(kind){
 case 32:case 47:case 51:case 53:case 74:case 177:case 178:case 187:case 188:case 189:
 case 194:case 203:case 204:case 205:case 226:case 231:case 232:case 233:return 1;
 default:return 0;
 }
}
static uint32_t actor_reservation_resource(int32_t kind)
{
 static const int32_t resource[26]={16,-1,24,20,11,25,12,10,14,18,22,23,9,28,21,17,29,30,-1,32,2,8,31,13,33,34};
 uint32_t index=(uint32_t)kind-1u;
 return index<26u?(uint32_t)resource[index]:UINT32_MAX;
}
void restore_actor_release(int32_t actor)
{
 uint32_t player=U(0xeaac28u)*0x49d8u,record=(uint32_t)actor*388u,home,target,current,resource,slot,index,group;
 int32_t kind=B(0xbcd602u+record),value,building_kind;
 if(kind==57)U(0xea5f04u+player)=U(0xea5f04u+player)+1u;
 if(kind==93||kind==100)U(0xea6210u+player)=U(0xea6210u+player)-1u;
 if(kind==92||kind==101||kind==77){slot=B(0xbcd68au+record);if(slot)U(0xbcba4cu+slot*144u)=0;}
 if(kind==5||kind==39){
  home=(uint32_t)H(0xbcd652u+record);if(home)W(0x799a04u+home*264u)=0;goto common;
 }
 if(kind==78||kind==77){(void)FUN_004a43a0(actor);goto common;}
 if(kind==62){home=(uint32_t)H(0xbcd652u+record);W(0x799a08u+home*264u)=0;goto common;}
 switch(kind){
 case 6:case 14:case 59:case 60:case 61:case 71:case 65:case 68:case 82:
 case 70:case 84:case 83:case 103:case 104:case 102:case 109:case 66:case 40:goto common;
 default:break;
 }
 if(kind==75||kind==90||(kind==73&&B(0x799a7eu+(uint32_t)H(0xbcd652u+record)*264u))){
  home=(uint32_t)H(0xbcd652u+record);B(0x799a7eu+home*264u)=(uint8_t)(B(0x799a7eu+home*264u)-1u);goto common;
 }
 if(kind==98){value=(int32_t)U(0xea61f0u+player);U(0xea61f0u+player)=value>0?(uint32_t)value-1u:0;goto common;}
 if(kind==15||kind==16||kind==17){
  home=(uint32_t)H(0xbcd652u+record);W(0x799a76u+home*264u+(uint32_t)(kind-15)*2u)=0;goto common;
 }
 if(kind==85){
  target=(uint32_t)H(0xbcd656u+record);
  if((uint8_t)restore_actor_building_gate((int32_t)target))W(0x799a8cu+target*264u)=0;
  goto common;
 }
 home=(uint32_t)H(0xbcd652u+record);
 if(kind==38){
  for(index=0;index<3u;++index)if(H(0x799a86u+home*264u+index*2u)==actor)W(0x799a86u+home*264u+index*2u)=0;
  goto common;
 }
 if(!home)goto common;
 if(H(0x799a02u+home*264u)==actor)W(0x799a02u+home*264u)=0;
 else if(H(0x799a8eu+home*264u)==actor)W(0x799a8eu+home*264u)=0;
 if(kind!=4&&kind!=9)goto common;
 target=(uint32_t)H(0xbcd656u+record);building_kind=H(0x7999e8u+target*264u);
 /* The reservation belongs to the global update cursor, which can differ
    from the actor being destroyed. Cache that cursor once as the native does. */
 current=U(0xd056f8u)*388u;
 if(actor_releases_reservation(building_kind)&&W(0xbcd66cu+current)){
  resource=actor_reservation_resource(H(0x799aa6u+(uint32_t)H(0xbcd656u+current)*264u));
  if(W(0x7999e8u+target*264u)==204u&&(uint32_t)B(0xbcd603u+record)==resource)
   W(0x799adeu+target*264u)=(uint16_t)(W(0x799adeu+target*264u)-1u);
  else W(0x799adcu+target*264u)=(uint16_t)(W(0x799adcu+target*264u)-1u);
  goto common;
 }
 target=(uint32_t)H(0xbcd656u+current);
 if(W(0x7999e8u+target*264u)==71u&&W(0xbcd66cu+current)){
  W(0x799adcu+target*264u)=(uint16_t)(W(0x799adcu+target*264u)-1u);
  W(0x799adeu+target*264u)=(uint16_t)(W(0x799adeu+target*264u)-W(0xbcd66cu+current));
 }
common:
 slot=B(0xbcd666u+record);
 if(slot){
  group=slot*106u;W(0x77de54u+group)=0;
  if(H(0x77de5au+group)==actor)W(0x77de5au+group)=0;
  else if(H(0x77de5cu+group)==actor)W(0x77de5cu+group)=0;
  else if(H(0x77de5eu+group)==actor)W(0x77de5eu+group)=0;
 }
 slot=U(0xbcd714u+record);if(slot)B(0x9784d8u+slot*44u)=0;
 if(W(0xbcd654u+record)){home=(uint32_t)H(0xbcd652u+record);W(0x799a06u+home*264u)=0;}
 FUN_004b3250(actor);FUN_004b5420(actor);
 for(index=0;index<97u;++index)U(0xbcd5f8u+record+index*4u)=0;
}
void FUN_004b4980(int32_t actor){restore_actor_release(actor);}
