#include "final_state_actions.h"
#define FINAL_H(a) ((int32_t)(int16_t)(*(volatile uint16_t *)(uintptr_t)(uint32_t)(a)))
extern void FUN_0046d100(int32_t group,uint32_t mode);
extern void FUN_004695d0(int32_t group,uint32_t mode);
extern void FUN_0053b090(uint32_t action);
extern void FUN_00516f90(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_005173a0(int32_t,int32_t,int32_t,int32_t);
uint32_t restore_select_highest_priority_group(void)
{
 uint32_t cursor=0xbcbad2u,list,i,record;int32_t highest=0,selected=0,score,id,x,y,radius;
 do {
  if (VIEW_B(cursor-122u)==1 && VIEW_B(cursor-56u)!=0) {
   score=FINAL_H(cursor);
   if (score>=highest) {selected=FINAL_H(cursor-116u);highest=score;}
  }
  cursor+=144u;
 } while (cursor<0xbcd662u);
 if (selected==0) return 0;
 VIEW_U(0xe950e0u)=0;list=0xbcb9d0u+(uint32_t)selected*144u;
 for (i=0;i<16;++i) {
  id=FINAL_H(list+i*2u);if (id==0) break;
  VIEW_B(0xbcd606u+(uint32_t)id*388u)=2;
 }
 FUN_0046d100(selected,0);FUN_004695d0(selected,1);FUN_0053b090(17);
 record=(uint32_t)selected*264u;
 y=FINAL_H(0x7999e0u+record);radius=VIEW_B(0x7999dbu+record);x=FINAL_H(0x7999deu+record);
 FUN_00516f90(x-radius,y-radius,x+radius,y+radius,1);
 y=FINAL_H(0x7999e0u+record);x=FINAL_H(0x7999deu+record);radius=VIEW_B(0x7999dbu+record);
 FUN_005173a0(x-radius,y-radius,x+radius,y+radius);
 return 1;
}
uint32_t FUN_004c41b0(void){return restore_select_highest_priority_group();}
