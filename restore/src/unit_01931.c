#include "view_state_actions.h"
extern void FUN_004e2970(uint32_t message,uint32_t first,uint32_t second);
void restore_city_check_notice_92(void)
{
 uint32_t city;
 if (VIEW_U(0x605a3cu)==1) return;
 if (VIEW_U(0xe38ebcu)==0 && VIEW_B(0xe38ea8u)==0) return;
 city=VIEW_U(0xeaac28u)*0x49d8u;
 if ((int32_t)VIEW_U(0xea1894u+city)<300) return;
 if ((int32_t)VIEW_U(0xea446cu+city)<=10) return;
 FUN_004e2970(92,0,0);
}
void FUN_004e2d80(void){restore_city_check_notice_92();}
