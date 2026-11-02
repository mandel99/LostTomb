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
int32_t FUN_00418140(void)
{
 if(CA_U(0xe38ebcu)==0){
  int32_t selected=(int8_t)CA_B(0xe38eabu);
  if(selected>=0&&selected<(int32_t)CA_U(0x6ff608u)){
   uint32_t record=0x6fdd08u+((uint32_t)selected<<6);
   if(record){
    uint8_t adjusted=CA_B(0xe39088u);int32_t value=(int32_t)CA_U(record+8u);
    if(!adjusted)return value;
    return (int32_t)((uint32_t)value+(value>=0x1b7?15u:200u));
   }
  }
 }
 {uint8_t level=CA_B(0xe38ea8u);return level==127?200:(int32_t)((uint32_t)(int32_t)(int8_t)level+200u);}
}
