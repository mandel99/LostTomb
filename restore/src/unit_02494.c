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
void FUN_0053b690(int32_t channel)
{
 if(CA_U(0xf55b88u)!=0&&CA_U(0xe92dd0u+(uint32_t)channel*4u)!=0&&FUN_0055f5a1(RESTORE_SOUND,channel)){
  int32_t volume=(int32_t)FUN_0055f5f4(RESTORE_SOUND,channel);
  while(volume>0){volume=(int32_t)((uint32_t)volume-5u);FUN_0055f639(RESTORE_SOUND,channel,volume);}
  FUN_0055f561(RESTORE_SOUND,channel);
 }
}
