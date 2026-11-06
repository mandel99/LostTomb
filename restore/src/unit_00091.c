#include "final_state_actions.h"
extern uint32_t FUN_00502780(void);
void restore_building_list_adjust_offset(void)
{
 uint32_t direction=VIEW_U(0xd3b198u),building=VIEW_U(0xeadd6cu),address=0x799ab8u+building*264u,value;
 if (direction==0) {
  value=VIEW_U(address)-1u;VIEW_U(address)=value;
  if ((int32_t)value<=0) VIEW_U(address)=1;
 } else {
  uint32_t rows,limit;
  VIEW_U(address)=VIEW_U(address)+1u;
  rows=FUN_00502780();
  building=VIEW_U(0xeadd6cu);value=VIEW_U(0x799ab8u+building*264u);
  value=rows+value-1u;limit=VIEW_U(0xf53e6cu);
  if ((int32_t)value>(int32_t)limit) {
   rows=FUN_00502780();limit=VIEW_U(0xf53e6cu);
   building=VIEW_U(0xeadd6cu);VIEW_U(0x799ab8u+building*264u)=limit-rows+1u;
  }
 }
 VIEW_B(0xe38dcau)=1;
}
void FUN_0040c6b0(void){restore_building_list_adjust_offset();}
