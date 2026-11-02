#include "campaign_map_input.h"
#include "campaign_actions.h"
#define MAP_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MAP_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define s_wavs_Fanfare_Nu1_wav_0057c030 (*(char (*)[21])0x0057c030u)
#define s_wavs_Fanfare_Nu5_wav_0057c018 (*(char (*)[21])0x0057c018u)
extern uint32_t FUN_0047d350(void);
/* The original passes three unused words to this authenticated zero-argument leaf. */
__declspec(noinline) void restore_campaign_map_fanfare(const char *path,uint32_t one,uint32_t zero)
{(void)path;(void)one;(void)zero;(void)FUN_0047d350();}
void FUN_00419090(void)
{
 uint32_t record,index=0,choice;
 int32_t selected;
 if(!MAP_B(0xe38e04u))return;
 selected=(int8_t)MAP_B(0xe38eabu);
 if(selected<0||selected>=(int32_t)MAP_U(0x6ff608u))restore_campaign_selection_failure(0x419090u);
 record=0x6fdd08u+(uint32_t)selected*64u;
 if((int32_t)MAP_U(record+0x38u)<=0)return;
 choice=record+0x24u;
 for(;;){
  uint32_t x=MAP_U(choice-4u),offset_x=MAP_U(0xe38da8u),offset_y=MAP_U(0xe38da4u);
  uint32_t y=MAP_U(choice);
  if(FUN_004cf620(x+offset_x,y+offset_y,44,44)){
   const char *path=s_wavs_Fanfare_Nu1_wav_0057c030;
   MAP_B(0x7027a4u)=(uint8_t)(index+1u);
   if(index&1u)path=s_wavs_Fanfare_Nu5_wav_0057c018;
   restore_campaign_map_fanfare(path,1,0);
   MAP_B(0xe38dcau)=1;return;
  }
  {uint32_t count=MAP_U(record+0x38u);index+=1u;choice+=12u;if((int32_t)index>=(int32_t)count)return;}
 }
}
