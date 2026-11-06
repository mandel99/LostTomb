#include "view_state_actions.h"
extern void FUN_00417ae0(uint32_t action);
extern void FUN_00442800(void);
extern void FUN_004cc110(uint32_t group,uint32_t entry);
extern void FUN_004cc160(uint32_t group,uint32_t entry);
extern void FUN_004e2970(uint32_t message,uint32_t first,uint32_t second);
extern void FUN_00538cd0(uint32_t first,uint32_t second,uint32_t third);
void restore_view_activate_primary_action(void)
{
 uint32_t mode=VIEW_U(0x605a3cu),state;
 VIEW_U(0x5d2e88u)=0xffffffffu;
 VIEW_U(0xf53fdcu)=0;
 if (mode==1u) {
  if (VIEW_U(0xe38e68u)==1u) {
   uint32_t second,first;
   FUN_004cc110(5,123);FUN_004cc160(5,124);
   second=VIEW_U(0xe20764u);first=VIEW_U(0xe20768u);
   FUN_00538cd0(first,second,0);VIEW_B(0xe38dcau)=1;return;
  }
  VIEW_B(0xe38ddau)=21;VIEW_U(0x775928u)=4;VIEW_U(0x775920u)=1;
  FUN_00442800();VIEW_B(0xe38dcau)=1;return;
 }
 state=VIEW_U(0xe38ebcu);
 if (state==0 && VIEW_U(0x78d828u)!=0) {FUN_004e2970(13,0,0);return;}
 VIEW_B(0xe38ddau)=20;
 if (state==0 && VIEW_B(0x7027c6u)==0 && (int32_t)VIEW_C(0xe38ea8u)==(int32_t)VIEW_U(0x579638u)) {
  VIEW_B(0xe38ddau)=1;FUN_00417ae0(14);VIEW_U(0xd3b17cu)=20;
 }
 VIEW_B(0xe38dcau)=1;
}
void FUN_00409fa0(void){restore_view_activate_primary_action();}
