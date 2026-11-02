#include "campaign_actions.h"
#include "campaign_index.h"
#include "campaign_profile.h"
#include "campaign_entry.h"
#include "family_menu.h"
#include "replay_mission.h"
#include "mission_menu.h"
#include "video_player.h"
#include "sound_streams.h"
#include <stdio.h>
#define CA_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define CA_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define CA_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define s_Binks_high_win_grand_big_bik_0057bfd8 (*(char (*)[29])0x0057bfd8u)
#define s_Binks_low_win_grand_small_bik_0057bff8 (*(char (*)[30])0x0057bff8u)
extern void FUN_004188f0(void);
__declspec(noinline) __declspec(noreturn) void restore_campaign_selection_failure(uint32_t origin)
{fprintf(stderr,"campaign selection invariant at %08lx\n",(unsigned long)origin);fflush(stderr);ExitProcess(origin);}
void FUN_00418220(void)
{
 uint8_t mode=CA_B(0xe38ddau);
 CA_U(0x7056c4u)=0;CA_U(0x7056c8u)=0;CA_U(0x7056b8u)=0;CA_U(0x7056c0u)=0;CA_U(0xe94e10u)=0;
 if(mode==0x36){FUN_00516c80();CA_B(0xe38dcau)=1;return;}
 if(mode==0x34){
  if(CA_B(0x7027a4u)!=0){
   uint32_t choice;int32_t previous,category,next;
   FUN_004188f0();choice=CA_U(0x7027a4u)&255u;
   previous=(int8_t)CA_B(0xe38eabu);CA_B(0xe38eaau)=(uint8_t)previous;
   category=(int8_t)CA_B(0xe38ea9u);
   next=FUN_0041b920(RESTORE_CAMPAIGN_INDEX,(uint32_t)category,previous,(int32_t)(choice-1u));
   CA_B(0xe38eabu)=(uint8_t)next;next=(int8_t)(uint8_t)next;
   if(next<0||next>=(int32_t)CA_U(0x6ff608u))restore_campaign_selection_failure(0x418220u);
   CA_B(0xe38ea8u)=CA_B(0x6fdd08u+((uint32_t)next<<6)+12u);
   FUN_004181e0();CA_B(0xe38dcau)=1;return;
  }
 }else{
  if(mode==0x35)CA_U(0xea5f8cu+CA_U(0xeaac28u)*0x49d8u)=0;
  CA_B(0xe38ddau)=1;CA_B(0xe38dd6u)=2;FUN_0053b690(1);
  {uint32_t address=0xea6018u+CA_U(0xeaac28u)*0x49d8u;int32_t limit=(int32_t)CA_U(0xe39080u),count=(int32_t)CA_U(address);
   CA_U(0xf55b94u)=0;if(limit<count)CA_U(address)=(uint32_t)limit;}
 }
 CA_B(0xe38dcau)=1;
}
