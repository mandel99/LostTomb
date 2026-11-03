#include "storage_transactions.h"
#include "storage_trade.h"
#include "storage_delivery.h"
#include "actor_civic.h"
#include "actor_transport.h"
#include "actor_walking.h"
#include "building_services.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

int32_t restore_storage_export(int32_t player,int32_t building,uint32_t trader)
{
 uint32_t node=(uint32_t)building,record,index,off=(uint32_t)player*0x49d8u,price,money;
 int32_t kind=H(0x7999e8u+node*264u),resource=0,amount;
 if(kind!=72&&kind!=193)return 0;
 for(index=0;index<8u;++index){
  node=(uint32_t)H(0x799a12u+node*264u);if(I(node)<=0)continue;
  record=node*264u;if(H(0x799a14u+record)<=0)continue;
  if(!FUN_004434e0(player,I(trader),H(0x7999eau+record)))continue;
  amount=FUN_0045d510(building,H(0x7999eau+record));if(amount<100)return 0;
  resource=H(0x7999eau+record);
 }
 if(!resource)return 0;
 (void)FUN_0045b570(player,building,resource,100);
 price=U(0x5d2c4cu+(uint32_t)resource*8u);
 if(I(U(0xea5e3cu+U(0xeaac28u)*0x49d8u))>0){
  amount=FUN_004cde40(I(price),150);money=U(0xea187cu+off)+(uint32_t)amount;
  /* Native snapshots the second price before publishing the cash balance. */
  price=U(0x5d2c4cu+(uint32_t)resource*8u);U(0xea187cu+off)=money;
  amount=FUN_004cde40(I(price),150);U(0xea44d4u+off)=U(0xea44d4u+off)+(uint32_t)amount;
 }else{
  U(0xea187cu+off)=U(0xea187cu+off)+price;U(0xea44d4u+off)=U(0xea44d4u+off)+price;
 }
 return resource;
}
void restore_trade_charge_import(int32_t player,uint32_t building,int32_t resource)
{
 uint32_t price=U(0x5d2c48u+(uint32_t)resource*8u),off=(uint32_t)player*0x49d8u;(void)building;
 U(0xea187cu+off)=U(0xea187cu+off)-price;U(0xea44dcu+off)=U(0xea44dcu+off)+price;
}
static void advance_resource(uint32_t at)
{
 uint32_t next=U(at)+1u;U(at)=next;if(I(next)>=36)U(at)=1;
}
int32_t restore_storage_import(int32_t player,int32_t building,uint32_t trader)
{
 uint32_t off=(uint32_t)player*0x49d8u,primary=0xea5b34u+off,secondary=0xea5b38u+off,index;
 int32_t resource,kind=H(0x7999e8u+(uint32_t)building*264u);
 if(kind!=72&&kind!=193)return 0;
 for(index=1;index<36u;++index){
  if(FUN_004435f0(player,I(trader),I(U(primary))))break;
  advance_resource(primary);
 }
 if(index>=36u)return 0;
 resource=I(U(primary));advance_resource(primary);
 if(FUN_0045b2a0(I(U(0xeaac28u)),building,resource,100)==100){
  FUN_0045d4d0(player,(uint32_t)building,resource);return resource;
 }
 for(index=1;index<36u;++index){
  advance_resource(secondary);
  if(!FUN_004435f0(player,I(trader),I(U(secondary))))continue;
  /* Secondary chooses eligibility; primary independently chooses delivery. */
  resource=I(U(primary));advance_resource(primary);
  if(FUN_0045b2a0(I(U(0xeaac28u)),building,resource,100)==100){
   FUN_0045d4d0(player,(uint32_t)building,resource);return resource;
  }
 }
 return 0;
}
int32_t restore_storage_distance(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t region1,int32_t region2)
{
 uint32_t penalty=region2<region1?(uint32_t)region1-(uint32_t)region2:(uint32_t)region2-(uint32_t)region1;
 if(region1==-1)penalty=0;
 U(0xd3b4fcu)=x2<x1?(uint32_t)x1-(uint32_t)x2:x1<x2?(uint32_t)x2-(uint32_t)x1:0;
 U(0xd3b4f8u)=y2<y1?(uint32_t)y1-(uint32_t)y2:y1<y2?(uint32_t)y2-(uint32_t)y1:0;
 return I((I(U(0xd3b4fcu))<I(U(0xd3b4f8u))?U(0xd3b4f8u):U(0xd3b4fcu))+penalty);
}
int32_t restore_storage_find_source(int32_t building,int32_t resource,char require_stock)
{
 uint32_t dest=(uint32_t)building*264u,slot,record,node,index,count,total,chosen=0;
 int32_t kind,best=10000,distance,source;int same_network=1,food=resource>=1&&resource<=8,eligible;
 if(U(0xea42f8u+U(0xeaac28u)*0x49d8u+(uint32_t)resource*4u))return 0;
 kind=H(0x7999e8u+dest);
 switch(kind){
 case 71:case 72:
  if(B(0x7900c2u+B(0x799a99u+dest)*196u+(uint32_t)resource)==2u)same_network=0;
  break;
 case 183:case 213:case 222:case 225:case 227:case 228:case 229:case 230:case 234:case 235:case 236:same_network=0;break;
 default:break;
 }
 count=U(0x78d788u);if(I(count)<1)return 0;
 for(slot=1;;++slot){
  record=slot*196u;source=H(0x7900bcu+record);
  if(B(0x7900c0u+record)&&source&&B(0x7999d8u+(uint32_t)source*264u)==1u&&source!=building){
   record=(uint32_t)source*264u;
   if(!same_network||B(0x7999ecu+record)==B(0x7999ecu+dest)){
    kind=H(0x7999e8u+record);eligible=0;
    if(kind==72){
     node=(uint32_t)source;total=0;
     for(index=0;index<8u;++index){
      node=(uint32_t)H(0x799a12u+node*264u);if(I(node)<=0)continue;
      if(H(0x799a14u+node*264u)>0&&H(0x7999eau+node*264u)==resource)total+=(uint32_t)H(0x799a14u+node*264u);
     }
     eligible=(I(total)>=400||!require_stock)&&I(total)>0&&(B(0x7900c2u+slot*196u+(uint32_t)resource)!=2u||require_stock);
    }else if(food&&kind==71){
     eligible=H(0x799a2eu+record+(uint32_t)resource*2u)>=400&&B(0x7900c2u+slot*196u+(uint32_t)resource)!=2u;
    }
    if(eligible){
     distance=FUN_0045dc50(H(0x7999deu+record),H(0x7999e0u+record),H(0x7999deu+dest),H(0x7999e0u+dest),H(0x7999f8u+dest),H(0x7999f8u+record));
     if(distance<best){best=distance;chosen=(uint32_t)source;}
    }
   }
  }
  if(I(slot+1u)>I(U(0x78d788u)))break;
 }
 if(I(chosen)<=0)return 0;
 record=chosen*264u;U(0xd3b4e0u)=(uint32_t)H(0x799a00u+record);U(0xd3b4e4u)=(uint32_t)H(0x7999feu+record);
 return I(chosen);
}
void restore_storage_rank_gathering(int32_t building,int32_t *first_resource,int32_t *second_resource,int32_t *first_percent,int32_t *second_percent,int32_t *first_source,int32_t *second_source)
{
 volatile int32_t *r1=first_resource,*r2=second_resource,*p1=first_percent,*p2=second_percent,*s1=first_source,*s2=second_source;
 uint32_t record=(uint32_t)building*264u,slot=B(0x799a99u+record)*196u,x=U(0xd3b4e4u),y=U(0xd3b4e0u),list,index;
 int32_t kind=H(0x7999e8u+record),count,resource,percent=100,values[36],amount,capacity,source;int gather=0;
 list=kind==71||kind==191?0xf53ddcu:0xf53e70u;count=I(U(list-4u));
 if(count<=0)return;
 for(index=0;I(index)<count;++index){
  resource=I(U(list+index*4u));values[resource]=100;
  if(B(0x7900c2u+slot+(uint32_t)resource)!=2u)continue;
  gather=1;
  if(W(0x7999e8u+record)==72u){
   amount=FUN_0045d510(building,resource);capacity=H(0x790134u+slot+(uint32_t)resource*2u);
   if(amount>=capacity)percent=100;
   else if(resource>0){amount=FUN_0045d510(building,resource);capacity=H(0x790134u+slot+(uint32_t)resource*2u);percent=I((uint32_t)amount*100u)/capacity;}
  }else{
   amount=H(0x799a2eu+record+(uint32_t)resource*2u);capacity=H(0x790134u+slot+(uint32_t)resource*2u);
   if(amount>=capacity)percent=100;else if(resource>0)percent=I((uint32_t)amount*100u)/capacity;
  }
  values[resource]=percent;
 }
 if(!gather)return;
 for(index=0;I(index)<count;++index){
  resource=I(U(list+index*4u));source=FUN_0045bb50(building,resource,0);
  if(U(0xea42f8u+U(0xeaac28u)*0x49d8u+(uint32_t)resource*4u)||!source)continue;
  percent=values[resource];
  if(percent<*p1){*p2=*p1;*r2=*r1;*s2=*s1;*p1=percent;*r1=resource;*s1=source;}
  else if(percent<*p2){*p2=percent;*r2=resource;*s2=source;}
 }
 U(0xd3b4e4u)=x;U(0xd3b4e0u)=y;
}
