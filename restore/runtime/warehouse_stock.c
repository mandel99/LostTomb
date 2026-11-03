#include "warehouse_stock.h"
#include "city_updates.h"
#include "menu_resource_neighbors.h"
#include "menu_prepare_integer.h"
#include "event_publisher.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

void restore_building_rebuild_bounds(void)
{
 uint32_t id,last=0,high;
 U(0x78d790u)=0;
 for(id=1;id<4000u;++id)if(B(0x7999d8u+id*264u))last=id;
 high=U(0x78d78cu);U(0x78d790u)=last;if(I(last)>I(high))U(0x78d78cu)=last;
 U(0x78d788u)=0;
 for(id=1;id<200u;++id)if(B(0x7900c0u+id*196u))U(0x78d788u)=id;
}
void restore_warehouse_update_stock(void)
{if(B(0xea624au))FUN_0045aae0(1);}
void restore_warehouse_rebuild_stock(int32_t player)
{
 uint32_t off=(uint32_t)player*0x49d8u,index,id,record,root,at;int32_t type,stock;uint8_t flag;
 for(index=0;index<36u;++index){U(0xea60d0u+off+index*4u)=0;U(0xea6160u+off+index*4u)=0;}
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;
  if(B(0x7999d8u+record)!=1u||(W(0x7999e8u+record)!=72u&&W(0x7999e8u+record)!=193u))continue;
  B(0x799a1cu+record)=0;
  if(FUN_00480180(H(0x7999deu+record),H(0x7999e0u+record),B(0x7999dbu+record)))B(0x799a1cu+record)=1;
  else if(FUN_00480180(H(0x7999deu+record),H(0x7999e0u+record),3))B(0x799a1cu+record)=2;
 }
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u||W(0x7999e8u+record)!=73u)continue;
  root=FUN_00475b50(id);flag=B(0x799a1cu+root*264u);if(!flag)continue;
  /* A disabled parent leaves the bin's old access flag intact. */
  B(0x799a1cu+record)=flag;type=H(0x7999eau+record);
  if(!type)U(0xea60d0u+off)=U(0xea60d0u+off)+400u;
  else{
   stock=H(0x799a14u+record);at=off+(uint32_t)type*4u;
   U(0xea60d0u+at)=U(0xea60d0u+at)+(400u-(uint32_t)stock);
   U(0xea6160u+at)=U(0xea6160u+at)+(uint32_t)stock;
  }
 }
 if((int32_t)(int8_t)B(0xe38ea8u)==I(U(0x5795acu))&&!B(0x7027c1u)&&!U(0xe38ebcu)&&I(U(0xea6194u+off))>=200)FUN_00417ae0(9);
 if((int32_t)(int8_t)B(0xe38ea8u)==I(U(0x5795c8u))&&!B(0x7027c2u)&&!U(0xe38ebcu)&&I(U(0xea619cu+off))>=300)FUN_00417ae0(10);
 if((int32_t)(int8_t)B(0xe38ea8u)==I(U(0x57961cu))&&!B(0x7027c5u)&&B(0x7027c4u)&&!U(0xe38ebcu)&&I(U(0xea6190u+off))>=800)FUN_00417ae0(13);
}
