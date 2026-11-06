#include "view_state_actions.h"
extern void FUN_00547a60(void);
void restore_view_toggle_tool(int32_t tool)
{
 uint8_t state=VIEW_B(0xe38ddau);
 if (state==30 || state==73 || state==31 || state==32) VIEW_B(0xe38ddau)=1;
 else if (state==18 || state==35) {if (VIEW_U(0xd3a6c4u)!=1) return;}
 else if (state!=1) return;
 if (VIEW_U(0xe94e0cu)!=(uint32_t)tool) {
  VIEW_U(0xe94e10u)=0;VIEW_U(0xe94e0cu)=(uint32_t)tool;
 } else {
  VIEW_U(0xe94e10u)=(uint32_t)tool;VIEW_U(0xe94e0cu)=0;
 }
 VIEW_B(0xe38dcau)=1;FUN_00547a60();
}
void FUN_004168e0(int32_t tool){restore_view_toggle_tool(tool);}
