#include "storage_delivery.h"
#include "menu_prepare_integer.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

static int32_t resource_limit(uint32_t slot,int32_t resource)
{
 uint32_t base=B(0x7900c2u+slot*196u+(uint32_t)resource)?0x790134u:0x7900ecu;
 return H(base+slot*196u+(uint32_t)resource*2u);
}
int32_t restore_storage_resource_amount(int32_t building,int32_t resource)
{
 uint32_t node=(uint32_t)building,total=0,index;int32_t phase;
 for(index=0;index<8u;++index){
  node=(uint32_t)H(0x799a12u+node*264u);if(I(node)<=0)return 0;
  phase=H(0x7999eau+node*264u);
  if(phase&&phase==resource)total+=(uint32_t)H(0x799a14u+node*264u);
 }
 return I(total);
}
uint32_t restore_storage_limit_exceeded(int32_t building,int32_t amount,int32_t resource)
{
 uint32_t slot=B(0x799a99u+(uint32_t)building*264u),total;int32_t limit=resource_limit(slot,resource);
 total=(uint32_t)FUN_0045d510(building,resource)+(uint32_t)amount;
 /* SETL writes AL only; public callers can observe the untouched high bytes. */
 return (total&0xffffff00u)|(uint32_t)(limit<I(total));
}
int32_t restore_storage_pile_frame(int32_t amount)
{
 if(amount==400)return 3;
 if(amount>=300&&amount<400)return 2;
 if(amount>=200&&amount<300)return 1;
 return 0;
}
int32_t restore_storage_deliver(int32_t player,int32_t building,int32_t resource,int32_t amount)
{
 uint32_t root=FUN_00475b50((uint32_t)building),slot=B(0x799a99u+root*264u),overflow=0,delivered=0;
 uint32_t node,chosen,index,record,counter,tile,image;int32_t limit,remaining,best,stock,phase,frame;
 if(H(0x799a18u+root*264u)<=0||W(0x799a22u+root*264u))return 0;
 if((uint8_t)FUN_0045b230(I(root),0,resource))return 0;
 /* Keep the original slot across both real capacity queries. */
 limit=resource_limit(slot,resource);
 if((uint8_t)FUN_0045b230(I(root),amount,resource))
  overflow=(uint32_t)FUN_0045d510(I(root),resource)+(uint32_t)amount-(uint32_t)limit;
 remaining=I((uint32_t)amount-overflow);if(remaining<=0)return remaining;
 do{
  node=FUN_00475b50((uint32_t)building);chosen=0;best=-1;
  for(index=0;index<8u;++index){
   node=(uint32_t)H(0x799a12u+node*264u);if(I(node)<=0)return 0;
   record=node*264u;phase=H(0x7999eau+record);
   if(phase&&phase!=resource)continue;
   stock=H(0x799a14u+record);
   if(stock<400&&stock>best){best=stock;chosen=node;}
  }
  if(!chosen)return 0;
  building=I(chosen);record=chosen*264u;counter=((uint32_t)player*0x1276u+(uint32_t)resource)*4u;
  U(0xea60d0u+counter)=U(0xea60d0u+counter)-1u;U(0xea6160u+counter)=U(0xea6160u+counter)+1u;
  W(0x7999eau+record)=(uint16_t)resource;W(0x799a14u+record)=(uint16_t)(W(0x799a14u+record)+1u);
  stock=H(0x799a14u+record);tile=U(0x7999e4u+record);
  image=U(resource<31?0xf566bcu:0xf57204u)+(uint32_t)resource*4u-(resource<31?4u:124u);
  frame=FUN_0045b490(stock);U(0xb7482cu+tile*4u)=image+(uint32_t)frame;
  ++delivered;
 }while(I(delivered)<remaining);
 return remaining;
}
