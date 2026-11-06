#include "final_state_actions.h"
extern void FUN_00536820(uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_00536c30(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004ccbb0(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_00536a30(uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004cc540(void);
extern void FUN_004cc770(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_00537140(uint32_t,uint32_t,uint32_t);
void restore_view_draw_text_entry(void)
{
 uint32_t a4=VIEW_U(0xe38da4u),a8=VIEW_U(0xe38da8u),selected,color;
 FUN_00536820(a8+10u,a4+160u,24,8);
 selected=VIEW_U(0x7027e4u)==1;
 a4=VIEW_U(0xe38da4u);a8=VIEW_U(0xe38da8u);
 FUN_00536c30(0,a8+50u,a4+246u,150,30,selected);
 a4=VIEW_U(0xe38da4u);color=VIEW_U(0xf56f8cu);a8=VIEW_U(0xe38da8u);
 FUN_004ccbb0(191,1,a8+55u,a4+256u,color,selected);
 a4=VIEW_U(0xe38da4u);a8=VIEW_U(0xe38da8u);
 FUN_00536a30(a8+42u,a4+208u,20,2);FUN_004cc540();
 a4=VIEW_U(0xe38da4u);color=VIEW_U(0xf56f9cu);a8=VIEW_U(0xe38da8u);
 FUN_004cc770(0x7027ecu,a8+58u,a4+216u,color,0);
 a4=VIEW_U(0xe38da4u);a8=VIEW_U(0xe38da8u);
 FUN_00537140(a8+58u,a4+217u,65535);
}
void FUN_0041d210(void){restore_view_draw_text_entry();}
