#include "industry_logistics.h"
#include "actor_transport.h"
#include "production_updates.h"
#include "menu_resource_neighbors.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

static int32_t material(int32_t resource)
{
 static const int8_t types[35]={-1,-1,21,-1,-1,-1,-1,-1,22,13,8,5,7,24,9,19,1,16,10,-1,4,15,11,12,3,6,-1,-1,14,17,18,23,20,25,26};
 return (uint32_t)resource<35u?types[resource]:-1;
}
static int32_t resource_of(int32_t type)
{
 static const int8_t resources[27]={-1,16,-1,24,20,11,25,12,10,14,18,22,23,9,28,21,17,29,30,-1,32,2,8,31,13,33,34};
 return (uint32_t)type<27u?resources[type]:-1;
}
static int32_t capacity(int32_t kind)
{
 switch(kind){
 case 32:case 51:return 300;
 case 47:case 53:return 500;
 case 84:case 85:case 86:case 87:case 184:case 187:case 188:case 189:return 100000;
 default:return 200;
 }
}
static int accepts_goods(int32_t kind)
{
 if(kind>=110&&kind<=114)return 1;
 switch(kind){
 case 32:case 47:case 51:case 53:case 74:case 177:case 178:case 187:case 188:case 189:
 case 194:case 203:case 204:case 205:case 226:case 231:case 232:case 233:return 1;
 default:return 0;
 }
}
int32_t restore_industry_delivery_destination(uint32_t actor,int32_t resource,uint32_t network)
{
 uint32_t id,record,count=0,selected,origin,tile;int32_t type,kind,stock,pending,best_stock=10000,best_pending=10000,limit,index,y;
 if(U(0xea42f8u+U(0xeaac28u)*0x49d8u+(uint32_t)resource*4u))return 0;
 type=material(resource);if(type<0)return 0;
 U(0x96e6b0u)=0;
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u)continue;
  if(FUN_00459240(I(id)))goto rejected;
  kind=H(0x7999e8u+record);
  if(!accepts_goods(kind)||!B(0x799a1cu+record)||H(0x7999f8u+record)<=0)goto rejected;
  if(H(0x7999eau+record)!=type&&H(0x799aa6u+record)!=type)goto rejected;
  if(B(0x7999ecu+record)!=network)goto rejected;
  if((kind==187||kind==188||kind==189)&&!W(0x799a18u+record))goto rejected;
  if(W(0x799aa6u+record)&&H(0x799aa6u+record)==type){stock=H(0x799aa4u+record);pending=H(0x799adeu+record);}
  else{stock=H(0x799a14u+record);pending=H(0x799adcu+record);}
  limit=capacity(kind);if(stock+100>limit||(pending>0&&limit-pending*100-stock<=0))goto rejected;
  if(pending>best_pending||stock>best_stock)goto rejected;
  if(stock<best_stock){count=0;best_stock=stock;best_pending=pending;}else count=U(0x96e6b0u);
  y=H(0x799a00u+record);origin=U(0xbca2d8u);U(0x972534u+count*4u)=id;
  tile=origin+(uint32_t)H(0x7999feu+record)+(uint32_t)y*228u;
  ++count;U(0x96e6b0u)=count;U(0x96e6b0u+count*4u)=tile;continue;
 rejected:count=U(0x96e6b0u);
 }
 index=FUN_004b28f0(I(actor),0x96e6b4,count);selected=index?U(0x972530u+(uint32_t)index*4u):0;record=selected*264u;
 U(0xd3b4e4u)=(uint32_t)H(0x7999feu+record);U(0xd3b4e0u)=(uint32_t)H(0x799a00u+record);
 if(selected){
  if(W(0x7999e8u+record)==204u&&resource==resource_of(H(0x799aa6u+record)))W(0x799adeu+record)=(uint16_t)(W(0x799adeu+record)+1u);
  else W(0x799adcu+record)=(uint16_t)(W(0x799adcu+record)+1u);
 }
 return I(selected);
}
int32_t restore_industry_producer_destination(uint32_t actor,int32_t resource,uint32_t network)
{
 uint32_t id,record,count=0,selected,origin,tile;int32_t type,index,y,bound;
 if(U(0xea42f8u+U(0xeaac28u)*0x49d8u+(uint32_t)resource*4u))return 0;
 type=material(resource);if(type<0)return 0;
 bound=I(U(0x78d790u));U(0x96e6b0u)=0;if(bound<1)return 0;
 for(id=1;;++id){
  record=id*264u;
  if(B(0x7999d8u+record)==1u&&accepts_goods(H(0x7999e8u+record))&&B(0x799a1cu+record)&&H(0x7999f8u+record)>0&&H(0x7999eau+record)==type&&B(0x7999ecu+record)==network){
   y=H(0x7999e0u+record);origin=U(0xbca2d8u);U(0x972534u+count*4u)=id;
   tile=origin+(uint32_t)H(0x7999deu+record)+(uint32_t)y*228u;++count;U(0x96e6b0u+count*4u)=tile;
  }
  if(I(id+1u)>I(U(0x78d790u)))break;
 }
 U(0x96e6b0u)=count;if(!count)return 0;
 index=FUN_004b28f0(I(actor),0x96e6b4,count);if(index<=0)return 0;
 selected=U(0x972530u+(uint32_t)index*4u);record=selected*264u;
 U(0xd3b4e4u)=(uint32_t)H(0x7999feu+record);U(0xd3b4e0u)=(uint32_t)H(0x799a00u+record);return I(selected);
}
int32_t restore_industry_city_destination(uint32_t unused1,uint32_t unused2,int32_t resource,uint32_t network)
{
 uint32_t selected=U(0xea5d1cu+U(0xeaac28u)*0x49d8u),record=selected*264u;int32_t type=material(resource),limit,stock,pending;
 (void)unused1;(void)unused2;
 if(resource!=10&&resource!=28)return 0;
 if(U(0xea42f8u+U(0xeaac28u)*0x49d8u+(uint32_t)resource*4u)||I(U(0x7058b8u))<=0)return 0;
 if(H(0x7999eau+record)==type){
  limit=capacity(H(0x7999e8u+record));stock=H(0x799a14u+record);if(stock>=limit)return 0;
  pending=H(0x799adcu+record);if(pending>0&&capacity(H(0x7999e8u+record))-pending*100-stock<=0)return 0;
 }
 if(H(0x799aa6u+record)==type){
  limit=capacity(H(0x7999e8u+record));stock=H(0x799aa4u+record);if(stock>=limit)return 0;
  pending=H(0x799adeu+record);if(pending>0&&capacity(H(0x7999e8u+record))-pending*100-stock<=0)return 0;
 }
 if(I(U(0xbcb9b4u))<=0)return 0;
 if(!FUN_00480180(H(0x7999deu+record),H(0x7999e0u+record),B(0x7999dbu+record)))return 0;
 if(B(0x7999ecu+record)!=network)return 0;
 if(H(0x7999eau+record)==type)W(0x799adcu+record)=(uint16_t)(W(0x799adcu+record)+1u);
 if(H(0x799aa6u+record)==type)W(0x799adeu+record)=(uint16_t)(W(0x799adeu+record)+1u);
 return I(selected);
}
int32_t restore_industry_deliver(int32_t building,int32_t amount,int32_t resource)
{
 uint32_t record=(uint32_t)building*264u,at=resource==10?0x799a14u:0x799aa4u,counter=resource==10?0x799adcu:0x799adeu;
 int32_t limit,stock;
 if(building<1)return 1;
 limit=capacity(H(0x7999e8u+record));stock=H(at+record);if(stock>=limit)return 0;
 /* Native returns/adds the excess after this wrapped sum, rather than space. */
 if(I((uint32_t)stock+(uint32_t)amount)>limit)amount=I((uint32_t)amount+(uint32_t)stock-(uint32_t)limit);
 W(at+record)=(uint16_t)((uint32_t)stock+(uint32_t)amount);W(counter+record)=(uint16_t)(W(counter+record)-1u);return amount;
}
int32_t FUN_0045f450(uint32_t actor,int32_t resource,uint32_t network){return restore_industry_producer_destination(actor,resource,network);}
