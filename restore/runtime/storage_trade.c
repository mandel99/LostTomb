#include "storage_trade.h"
#include "storage_delivery.h"
#include "training_queries.h"
#include "menu_list_scroll.h"
#include "menu_resource_neighbors.h"
#include "actor_transport.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

int32_t restore_trade_can_export(int32_t player,int32_t trader,int32_t resource)
{
 uint32_t off=(uint32_t)player*0x49d8u+(uint32_t)resource*2u,trade=(uint32_t)trader*106u,table;
 int32_t quota,stock;
 if(W(0xea4096u+off)!=2u&&W(0xea4096u+off)!=4u)return 0;
 if(resource==27||resource<0||resource>=36)return 0;
 table=((uint32_t)resource+B(0x77de04u+trade)*36u)*4u;
 quota=FUN_004c5240(I(U(0x77646cu+table)));
 if(trader){
  table=((uint32_t)resource+B(0x77de04u+trade)*36u)*4u;
  if(I(U(0x77592cu+table))>=quota)return 0;
 }
 stock=I(U(0xea6160u+(uint32_t)player*0x49d8u+(uint32_t)resource*4u));
 if(W(0xea4096u+off)==2u){if(stock<=H(0xea40deu+off)*100)return 0;}
 else if(stock<=I(U(0x775810u+(uint32_t)resource*4u)))return 0;
 if(trader&&!B(0x77de06u+trade+(uint32_t)resource))return 0;
 return 1;
}
int32_t restore_trade_can_import(int32_t player,int32_t trader,int32_t resource)
{
 uint32_t off,trade,table;int32_t quota,stock;
 if(I(U(0xea187cu+U(0xeaac28u)*0x49d8u))<=-5000)return 0;
 off=(uint32_t)player*0x49d8u+(uint32_t)resource*2u;
 if(W(0xea4096u+off)!=1u&&W(0xea4096u+off)!=3u)return 0;
 if(resource==27||resource<0||resource>=36)return 0;
 trade=(uint32_t)trader*106u;if(!B(0x77de2au+trade+(uint32_t)resource))return 0;
 table=((uint32_t)resource+B(0x77de04u+trade)*36u)*4u;
 quota=FUN_004c5240(I(U(0x77646cu+table)));
 table=((uint32_t)resource+B(0x77de04u+trade)*36u)*4u;
 if(I(U(0x77592cu+table))>=quota)return 0;
 stock=I(U(0xea6160u+(uint32_t)player*0x49d8u+(uint32_t)resource*4u));
 if(resource<1)return 0;
 if(W(0xea4096u+off)==1u)return stock<H(0xea40deu+off)*100;
 return stock<I(U(0x775810u+(uint32_t)resource*4u));
}
int32_t restore_storage_can_export(int32_t actor,int32_t player,int32_t building,int32_t trader)
{
 uint32_t node=(uint32_t)building,index,record;int32_t kind=H(0x7999e8u+node*264u);
 if((kind!=72&&kind!=193)||B(0xbcd667u+(uint32_t)actor*388u)>=8u)return 0;
 for(index=0;index<8u;++index){
  node=(uint32_t)H(0x799a12u+node*264u);if(I(node)<=0)continue;
  record=node*264u;if(H(0x799a14u+record)<=0)continue;
  if(FUN_004434e0(player,trader,H(0x7999eau+record))&&
     FUN_0045d510(building,H(0x7999eau+record))>=100)return 1;
 }
 return 0;
}
int32_t restore_storage_can_import(int32_t actor,int32_t player,int32_t building,int32_t trader)
{
 uint32_t node=(uint32_t)building,slot,index,cursor_at,cursor,record;int32_t kind=H(0x7999e8u+node*264u),resource,eligible=0,allowed;
 uint8_t policy;
 if(kind!=72&&kind!=193)return 0;
 (void)FUN_004cde60(H(0x799a18u+node*264u),I(U(0x705eb8u+(uint32_t)kind*40u)));
 if(H(0xbcd66cu+(uint32_t)actor*388u)>=8)return 0;
 slot=B(0x799a99u+node*264u)*196u;
 for(resource=1;resource<36;++resource){
  policy=B(0x7900c2u+slot+(uint32_t)resource);
  if(policy!=1u&&policy!=3u&&!(uint8_t)FUN_0045b230(building,0,resource)&&
     !(uint8_t)FUN_0045b230(building,100,resource)&&FUN_004435f0(player,trader,resource))++eligible;
 }
 if(eligible<=0)return 0;
 cursor_at=0xea5b34u+(uint32_t)player*0x49d8u;
 allowed=FUN_004435f0(player,trader,I(U(cursor_at)));
 policy=B(0x7900c2u+slot+U(0xea5b34u+U(0xeaac28u)*0x49d8u));
 if(policy==1u||!allowed){
  for(index=1;index<36u;++index){
   cursor=U(cursor_at)+1u;U(cursor_at)=cursor;if(I(cursor)>=36)U(cursor_at)=1;
   policy=B(0x7900c2u+slot+U(0xea5b34u+U(0xeaac28u)*0x49d8u));
   if(policy!=1u&&FUN_004435f0(player,trader,I(U(cursor_at))))break;
  }
  if(index==36u)return 0;
 }
 for(index=0;index<8u;++index){
  node=(uint32_t)H(0x799a12u+node*264u);if(I(node)<=0)continue;
  record=node*264u;
  if(H(0x799a14u+record)<400&&(!W(0x799a14u+record)||FUN_004435f0(player,trader,H(0x7999eau+record))))return 1;
 }
 return 0;
}
int32_t restore_food_delivery_destination(int32_t force,int32_t actor,int32_t resource,uint32_t network)
{
 uint32_t id,record,slot,limit_at,count,tile,destination;int32_t kind,stock,capacity,index;uint8_t policy;
 if(U(0x784bbcu)||resource<1||resource>8)return 0;
 if(U(0xea42f8u+U(0xeaac28u)*0x49d8u+(uint32_t)resource*4u)&&!force)return 0;
 /* Snapshot the first bound before resetting the shared candidate count. */
 index=I(U(0x78d790u));U(0x96e6b0u)=0;if(index<1)return 0;
 for(id=1;;++id){
  record=id*264u;kind=H(0x7999e8u+record);
  if(B(0x7999d8u+record)==1u&&(kind==71||kind==191)&&B(0x799a1cu+record)&&
     H(0x7999f8u+record)>0&&B(0x7999ecu+record)==network){
   if(FUN_004cde60(H(0x799a18u+record),I(U(0x705eb8u+(uint32_t)kind*40u)))<100)U(0xd05628u)=U(0xd05628u)+1u;
   else{
    slot=B(0x799a99u+record)*196u;policy=B(0x7900c2u+slot+(uint32_t)resource);
    if(policy==0u||policy==2u){
     stock=H(0x799a2eu+record+(uint32_t)resource*2u);
     limit_at=(policy==0u?0x7900ecu:0x790134u)+slot+(uint32_t)resource*2u;capacity=H(limit_at);
     if(stock<capacity&&(H(0x799adcu+record)<=0||H(limit_at)-H(0x799adeu+record)-stock>0)&&H(0x799a2eu+record)>=100){
      U(0x972534u+U(0x96e6b0u)*4u)=id;
      (void)FUN_004806a0(H(0x7999deu+record),H(0x7999e0u+record));
      tile=U(0xd3b4e0u)*228u;tile+=U(0xd3b4e4u);tile+=U(0xbca2d8u);
      count=U(0x96e6b0u);U(0x96e6b4u+count*4u)=tile;U(0x96e6b0u)=count+1u;
     }
    }
   }
  }
  if(I(id+1u)>I(U(0x78d790u)))break;
 }
 count=U(0x96e6b0u);if(!count)return 0;
 index=FUN_004b28f0(actor,0x96e6b4,count);if(!index)return 0;
 destination=U(0x972530u+(uint32_t)index*4u);record=destination*264u;
 U(0xd3b4e4u)=(uint32_t)H(0x7999feu+record);U(0xd3b4e0u)=(uint32_t)H(0x799a00u+record);
 if(destination){W(0x799adcu+record)=(uint16_t)(W(0x799adcu+record)+1u);
  W(0x799adeu+record)=(uint16_t)(W(0x799adeu+record)+W(0xbcd66cu+(uint32_t)actor*388u));}
 return I(destination);
}
