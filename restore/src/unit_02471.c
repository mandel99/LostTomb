#include "view_state_actions.h"
#include "media_bindings.h"
extern void FUN_004ce100(uint32_t state);
extern void FUN_004ce2d0(void);
void restore_view_close_video(void)
{
 uint32_t handle;
 if (VIEW_U(0xf54054u)==0) return;
 if (VIEW_U(0xf5404cu)==0) FUN_004ce100(0xf5400cu);
 handle=VIEW_U(0xf54114u);
 if (handle!=0) {(void)_SmackClose_4((uintptr_t)handle);VIEW_U(0xf54114u)=0;}
 FUN_004ce2d0();
}
void FUN_005391e0(void){restore_view_close_video();}
