#include "view_state_actions.h"
#include "menu_tail_ui.h"
extern void FUN_004d6250(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
void restore_view_draw_setting_toggle(void)
{
 uint32_t a4=VIEW_U(0xe38da4u),a8=VIEW_U(0xe38da8u),enabled=VIEW_U(0x7056c8u)==1;
 uint32_t fresh_a4,fresh_a8,color;
 FUN_004d63d0(0,a8+240u,a4+176u,160,32,enabled,a8,a4);
 fresh_a4=VIEW_U(0xe38da4u);fresh_a8=VIEW_U(0xe38da8u);color=VIEW_U(0xf56f8cu);
 FUN_004d6250(18,3,a8+240u,a4+187u,160,color,0,fresh_a8,fresh_a4);
}
void FUN_005345a0(void){restore_view_draw_setting_toggle();}
