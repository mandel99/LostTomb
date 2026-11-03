#include "granary.h"
#include "actor_transport.h"
#include "building_services.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

int32_t restore_granary_gather_destination(int32_t actor,int32_t resource,uint32_t network)
{
 uint32_t id,record,slot,count,tile,selected,origin;int32_t kind,index,y;
 if(U(0x784bbcu)||resource<1||resource>8)return 0;
 if(U(0xea42f8u+U(0xeaac28u)*0x49d8u+(uint32_t)resource*4u))return 0;
 index=I(U(0x78d790u));U(0x96e6b0u)=0;if(index<1)return 0;
 count=0;
 for(id=1;;++id){
  record=id*264u;kind=H(0x7999e8u+record);
  if(B(0x7999d8u+record)==1u&&(kind==71||kind==191)&&B(0x799a1cu+record)&&H(0x7999f8u+record)>0&&B(0x7999ecu+record)==network){
   if(FUN_004cde60(H(0x799a18u+record),I(U(0x705eb8u+(uint32_t)kind*40u)))>=100){
    slot=B(0x799a99u+record)*196u;
    if(B(0x7900c2u+slot+(uint32_t)resource)==2u&&H(0x790134u+slot+(uint32_t)resource*2u)>H(0x799a2eu+record+(uint32_t)resource*2u)&&H(0x799a2eu+record)>100){
     y=H(0x7999e0u+record);origin=U(0xbca2d8u);count=U(0x96e6b0u);
     U(0x972534u+count*4u)=id;tile=origin+(uint32_t)H(0x7999deu+record)+(uint32_t)y*228u+229u;
     ++count;U(0x96e6b0u)=count;U(0x96e6b0u+count*4u)=tile;
    }else count=U(0x96e6b0u);
   }else count=U(0x96e6b0u);
  }
  if(I(id+1u)>I(U(0x78d790u)))break;
 }
 if(!count)return 0;
 index=FUN_004b28f0(actor,0x96e6b4,count);if(!index)return 0;
 selected=U(0x972530u+(uint32_t)index*4u);record=selected*264u;
 U(0xd3b4e0u)=(uint32_t)(H(0x7999e0u+record)+1);U(0xd3b4e4u)=(uint32_t)(H(0x7999deu+record)+1);
 return I(selected);
}
int32_t restore_granary_find_gather_source(int32_t building)
{
 uint32_t dest=(uint32_t)building*264u,slot=B(0x799a99u+dest)*196u,index,record,source_slot,total,selected=0,count;
 int32_t resource,gather=0,source,best=10000,distance;
 for(resource=1;resource<=8;++resource)if(B(0x7900c2u+slot+(uint32_t)resource)==2u)++gather;
 if(U(0x784bbcu)||gather<=0)return 0;
 count=U(0x78d9a0u);
 for(index=0;I(index)<I(count);++index){
  source=H(0x78d8d4u+index*2u);record=(uint32_t)source*264u;
  if(B(0x7999ecu+record)==B(0x7999ecu+dest)){
   source_slot=B(0x799a99u+record)*196u;total=0;
   for(resource=1;resource<=8;++resource)
    if(B(0x7900c2u+slot+(uint32_t)resource)==2u&&B(0x7900c2u+source_slot+(uint32_t)resource)!=2u)total+=(uint32_t)H(0x799a2eu+record+(uint32_t)resource*2u);
   if(I(total)>0){
    /* Native deliberately passes the destination X coordinate twice. */
    distance=FUN_0045dc50(H(0x7999feu+record),H(0x799a00u+record),H(0x7999feu+dest),H(0x7999feu+dest),H(0x7999f8u+dest),H(0x7999f8u+record));
    if(I(total)<=400)distance=I((uint32_t)distance*2u);
    if(distance<best){best=distance;selected=(uint32_t)source;}
   }
  }
  count=U(0x78d9a0u);
 }
 record=selected*264u;U(0xd3b4e0u)=(uint32_t)H(0x799a00u+record);U(0xd3b4e4u)=(uint32_t)H(0x7999feu+record);
 return I(selected);
}
void restore_granary_rebuild_supply(void)
{
 uint32_t index,id,record,slot,total,out=0x78d8d4u;int32_t resource,kind;
 for(index=0;index<50u;++index)U(0x78d8d4u+index*4u)=0;
 for(index=0;index<36u;++index)U(0x78d844u+index*4u)=0;
 U(0x78d9a0u)=0;
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;kind=H(0x7999e8u+record);
  if(B(0x7999d8u+record)!=1u||(kind!=71&&kind!=191)||!B(0x799a1cu+record)||H(0x7999f8u+record)<=0)continue;
  slot=B(0x799a99u+record)*196u;total=0;
  for(resource=1;resource<=8;++resource){
   if(B(0x7900c2u+slot+(uint32_t)resource)==2u)continue;
   total+=(uint32_t)H(0x799a2eu+record+(uint32_t)resource*2u);
   U(0x78d844u+(uint32_t)resource*4u)=U(0x78d844u+(uint32_t)resource*4u)+(uint32_t)H(0x799a2eu+record+(uint32_t)resource*2u);
  }
  if(I(total)>100){W(out)=(uint16_t)id;if(out<0x78d998u){U(0x78d9a0u)=U(0x78d9a0u)+1u;out+=2u;}}
 }
}
int32_t restore_granary_take_gathered(int32_t destination,int32_t source)
{
 uint32_t dest=(uint32_t)destination*264u,src=(uint32_t)source*264u,slot=B(0x799a99u+dest)*196u,source_slot=B(0x799a99u+src)*196u;
 int32_t resource,chosen=0,amount=0,stock;
 for(resource=1;resource<=8;++resource){
  if(B(0x7900c2u+slot+(uint32_t)resource)==2u&&B(0x7900c2u+source_slot+(uint32_t)resource)!=2u){
   stock=H(0x799a2eu+src+(uint32_t)resource*2u);if(stock>amount){chosen=resource;amount=stock;}
  }
 }
 if(amount>800)amount=800;if(H(0x799a2eu+dest)<amount)amount=H(0x799a2eu+dest);
 (void)FUN_0045e360(I(U(0xeaac28u)),source,chosen,amount);U(0x78d99cu)=(uint32_t)chosen;
 return amount;
}
int32_t restore_granary_deliver(int32_t building,int32_t resource,int32_t produced,int32_t amount)
{
 uint32_t record=(uint32_t)building*264u,slot=B(0x799a99u+record)*196u,counter,at;int32_t kind,free,result;uint8_t policy;
 if(building<1)return 1;kind=H(0x7999e8u+record);
 if((kind!=71&&kind!=191)||resource<1||resource>8)return 0;
 free=H(0x799a2eu+record);if(free<=0)return 0;
 policy=B(0x7900c2u+slot+(uint32_t)resource);at=0x799a2eu+record+(uint32_t)resource*2u;
 if(policy==0u){if(H(at)>=H(0x7900ecu+slot+(uint32_t)resource*2u))return 0;}
 else if(policy==2u){if(H(at)>=H(0x790134u+slot+(uint32_t)resource*2u))return 0;}
 if(produced){counter=0xea5e54u+U(0xeaac28u)*0x49d8u;U(counter)=U(counter)+(uint32_t)amount;}
 if(amount<free){W(at)=(uint16_t)(W(at)+(uint32_t)amount);W(0x799a2eu+record)=(uint16_t)(W(0x799a2eu+record)-(uint32_t)amount);return amount;}
 W(at)=(uint16_t)(W(at)+(uint32_t)free);result=H(0x799a2eu+record);W(0x799a2eu+record)=0;return result;
}
int32_t restore_granary_withdraw(int32_t player,int32_t building,int32_t resource,int32_t amount)
{
 uint32_t record=(uint32_t)building*264u,at=0x799a2eu+record+(uint32_t)resource*2u,counter,remaining,value;int32_t stock;
 if(amount<=0)return 0;stock=H(at);counter=0xea60acu+(uint32_t)player*0x49d8u+(uint32_t)resource*4u;
 if(stock<=amount){
  remaining=(uint32_t)amount-(uint32_t)stock;value=U(counter)-(uint32_t)stock;
  W(0x799a2eu+record)=(uint16_t)(W(0x799a2eu+record)+(uint32_t)stock);W(at)=0;U(counter)=value;return I(remaining);
 }
 W(at)=(uint16_t)((uint32_t)stock-(uint32_t)amount);U(counter)=U(counter)-(uint32_t)amount;
 W(0x799a2eu+record)=(uint16_t)(W(0x799a2eu+record)+(uint32_t)amount);return 0;
}
int32_t FUN_0045dcc0(int32_t actor,int32_t resource,uint32_t network){return restore_granary_gather_destination(actor,resource,network);}
int32_t FUN_0045de80(int32_t building){return restore_granary_find_gather_source(building);}
int32_t FUN_0045e150(int32_t destination,int32_t source){return restore_granary_take_gathered(destination,source);}
