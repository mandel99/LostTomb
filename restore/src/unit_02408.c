#include "view_state_actions.h"
extern void FUN_004d6250(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
void restore_view_draw_selected_entry(void)
{
 uint32_t selected,id,a4,a8,color,width,height,item;uint8_t active;
 if (VIEW_U(0x605a3cu)==1) return;
 selected=VIEW_U(0x784f14u);
 if ((int32_t)selected<=0 || VIEW_B(0x77f6e6u+selected*98u)!=1) return;
 active=VIEW_B(0xe39088u);id=active ? 21u : 195u;
 a4=VIEW_U(0xe38da4u);a8=VIEW_U(0xe38da8u);color=VIEW_U(0xf56fb4u);
 if (active) {width=VIEW_U(0xe38dacu);height=VIEW_U(0xe38db0u);}
 else {height=VIEW_U(0xe38db0u);width=VIEW_U(0xe38dacu);}
 item=VIEW_B(0x77de03u+VIEW_U(0x784f18u)*106u);
 FUN_004d6250(id,item,(uint32_t)((int32_t)(height-332u)/2),width-118u,332,color,0,a8,a4);
}
void FUN_0052d940(void){restore_view_draw_selected_entry();}
