#include "view_state_actions.h"
extern void FUN_004e2970(uint32_t message,uint32_t first,uint32_t second);
void restore_city_check_notice_93(void)
{
 uint32_t city;int32_t half,other;
 if (VIEW_U(0x605a3cu)==1) return;
 city=VIEW_U(0xeaac28u)*0x49d8u;
 if ((int32_t)VIEW_U(0xea1894u+city)<300) return;
 half=(int32_t)VIEW_U(0xea5e1cu+city)/2;
 other=(int32_t)VIEW_U(0xea41dcu+city);
 if (other<=half) FUN_004e2970(93,0,0);
}
void FUN_004e2dd0(void){restore_city_check_notice_93();}
