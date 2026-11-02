#include "campaign_actions.h"
#include "campaign_index.h"
#include "campaign_profile.h"
#include "campaign_entry.h"
#include "family_menu.h"
#include "replay_mission.h"
#include "mission_menu.h"
#include "video_player.h"
#include "sound_streams.h"
#include "game_diagnostics.h"
#include <stdio.h>
#define CA_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define CA_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define CA_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define s_Binks_high_win_grand_big_bik_0057bfd8 (*(char (*)[29])0x0057bfd8u)
#define s_Binks_low_win_grand_small_bik_0057bff8 (*(char (*)[30])0x0057bff8u)
extern void FUN_004188f0(void);
void FUN_00418640(void)
{
 uint32_t offset=CA_U(0xeaac28u)*0x49d8u,first=CA_U(0xea5b48u+offset),second=CA_U(0xea5cc0u+offset),kind=CA_U(0xe38ebcu),runs;
 int32_t next=-1;uint8_t pending=0;
 CA_U(0xe38eb0u)=first;runs=CA_U(0xe38eb8u)+1u;CA_U(0xe38eb4u)=second;CA_U(0xe38eb8u)=runs;
 if(kind==0){
  int32_t selected=(int8_t)CA_B(0xe38eabu);
  if(selected>=0&&selected<(int32_t)CA_U(0x6ff608u)){
   uint32_t record=0x6fdd08u+((uint32_t)selected<<6);
   if(record){char *path;CA_B(record+0x3cu)=1;path=FUN_00516a20();(void)FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,path);}
  }
  selected=(int8_t)CA_B(0xe38eabu);
  next=FUN_0041b920(RESTORE_CAMPAIGN_INDEX,(uint32_t)(int32_t)(int8_t)CA_B(0xe38ea9u),selected,0);
  restore_game_trace("mission advance selected=%d next=%d scene=%d category=%d kind=%u\n",
      selected,next,(int32_t)(int8_t)CA_B(0xe38ea8u),(int32_t)(int8_t)CA_B(0xe38ea9u),(uint32_t)CA_B(0x7027b4u));
  {uint8_t state=CA_B(0x7027b4u);
   if((state==2||state==1)&&next>=0){
    uint8_t previous;int32_t current,count;
    FUN_004188f0();previous=CA_B(0xe38eabu);current=(int8_t)previous;
    if(current>=0){
     count=(int32_t)CA_U(0x6ff608u);
     if(current<count){
      uint32_t record=0x6fdd08u+((uint32_t)current<<6);
      if(record){
       if((int32_t)CA_U(record+0x38u)<2){
        CA_B(0xe38eaau)=previous;CA_B(0xe38eabu)=(uint8_t)next;
        if(next>=count)restore_campaign_selection_failure(0x418640u);
        CA_B(0xe38ea8u)=CA_B(0x6fdd08u+((uint32_t)next<<6)+12u);
       }else pending=1;
      }
     }
    }
   }else if(CA_B(0xe38ea8u)==0x28){
    uint32_t record=0x799ae6u;char *path;
    do{if(CA_W(record+10u)==0xebu){CA_W(0x702794u)=(uint16_t)(CA_W(record)+1u);CA_W(0x702796u)=(uint16_t)(CA_W(record+2u)+0x11u);}record+=0x108u;}while((int32_t)record<0x89b6de);
    path=FUN_00516a20();(void)FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,path);
   }
  }
 }
 offset=CA_U(0xeaac28u)*0x49d8u;CA_U(0xea5d40u+offset)=0;CA_U(0xea5d44u+offset)=0;CA_U(0xea5d48u+offset)=0;kind=CA_U(0xe38ebcu);
 if(kind!=0){
  CA_U(0x605a3cu)=0;CA_B(0x6de515u)=0;CA_U(0x704db0u)=0;CA_U(0x704dacu)=3;FUN_0041f870();
 }else if(next>=0&&CA_B(0x7027b4u)!=3){
  if(pending){CA_B(0xe38ddau)=0x34;CA_B(0x7027a4u)=0;}else FUN_004181e0();
 }else{
  if(CA_B(0x7027b4u)==1){
   int32_t chapter=(int32_t)(int8_t)CA_B(0xe38ea9u)+1;CA_U(0x7027b0u)=(uint32_t)chapter;
   if(chapter!=5){
    if(chapter<9){CA_U(0x7027acu)=(uint32_t)chapter;CA_B(0xe38ddau)=0x17;CA_U(0x6fdc98u)=0;goto done;}
   }else{
    const char *movie=(int32_t)CA_U(0xe38edcu)<0xe9?s_Binks_low_win_grand_small_bik_0057bff8:s_Binks_high_win_grand_big_bik_0057bfd8;
    int32_t y=(int32_t)(CA_U(0xe38dacu)-286u)/2,x=(int32_t)(CA_U(0xe38db0u)-480u)/2;
    RestoreVideoPlayer *player=RESTORE_CURRENT_VIDEO;(void)FUN_00413580(player,movie,x,y);
   }
  }
  FUN_004cb6a0();
 }
done:
 restore_game_trace("mission advance published selected=%d scene=%d mode=%u briefing=%u\n",
     (int32_t)(int8_t)CA_B(0xe38eabu),(int32_t)(int8_t)CA_B(0xe38ea8u),(uint32_t)CA_B(0xe38ddau),CA_U(0xd3b124u));
 CA_U(0xbc9694u)=0;CA_U(0xe94e0cu)=0;CA_B(0xe38dcau)=1;
}
