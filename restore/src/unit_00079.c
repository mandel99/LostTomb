#include "view_state_actions.h"
extern void FUN_0040a090(void);
extern void FUN_004e2970(uint32_t message,uint32_t first,uint32_t second);
void restore_view_toggle_primary_action(void)
{
 if (VIEW_U(0x605a3cu)!=1 && VIEW_U(0xe38ebcu)==0 && VIEW_U(0x78d828u)!=0) {
  FUN_004e2970(13,0,0);return;
 }
 if (VIEW_B(0xe38ddau)==1) {FUN_00409fa0();return;}
 FUN_0040a090();
}
void FUN_0040b590(void){restore_view_toggle_primary_action();}
