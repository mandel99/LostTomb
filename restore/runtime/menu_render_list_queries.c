/* Pinned native 5027B0(59) and521060(5), literal width/order restoration. */
#include "menu_render_list_queries.h"
#include "menu_list_scroll.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
int32_t FUN_005027b0(void)
{
 uint32_t building=U(0xeadd6cu),count,index=0,eligible=0,record=0xf53e70u;int32_t limit;
 uint16_t kind=H(0x7999e8u+building*264u);
 if(kind==0x48u||kind==0xc1u){
  limit=(int32_t)FUN_00502780();count=U(0xf53e6cu);
  if(limit<(int32_t)count)return (int32_t)FUN_00502780();
  return (int32_t)count;
 }
 if(kind!=0x46u&&kind!=0xbeu)return (int32_t)U(0xf53dd8u);
 count=U(0xf53e6cu);
 if((int32_t)count>0){
  do {
   if(FUN_005013a0(U(record)))++eligible;
   count=U(0xf53e6cu);++index;record+=4u;
  }while((int32_t)index<(int32_t)count);
 }
 return (int32_t)eligible;
}
void FUN_00521060(void)
{
 if((int32_t)U(0xf53f48u)<=5){U(0xf53f48u)=6;B(0xe38dcau)=1;}
}
