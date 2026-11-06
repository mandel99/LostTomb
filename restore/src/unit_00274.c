#include "final_state_actions.h"
#include "campaign_profile.h"
#include "campaign_unlock.h"
#include <windows.h>
extern void FUN_0041d3d0(void);
extern void FUN_004ceed0(const char *path);
extern const char *FUN_00516a20(void);
uint8_t restore_window_handle_debug_key(uint32_t message,uint32_t key)
{
 uint16_t alt=(uint16_t)GetAsyncKeyState(18)&0xff00u;
 if (VIEW_B(0xe38ddau)!=1) return 0;
 if (message!=0x100u) {
  if (message!=0x104u) return 0;
  if (key==0x31u) FUN_004ceed0((const char *)0x57c768u);
  else if (key==0x32u) FUN_004ceed0((const char *)0x57c774u);
 }
 switch (key) {
 case 0x43:case 0x63:
  if (VIEW_U(0xe92e20u)!=0 && alt!=0) {FUN_0041d3d0();return 1;}
  break;
 case 0x46:case 0x66:
  if (VIEW_U(0xe92e20u)!=0 && alt!=0 && VIEW_B(0xe38e74u)!=0 && VIEW_B(0xe38ddau)!=0x36) {
   const char *path;
   restore_campaign_unlock_all(RESTORE_CAMPAIGN_INDEX);
   path=FUN_00516a20();
   (void)FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,path);
   return 1;
  }
 }
 return 0;
}
uint8_t FUN_0041d110(uint32_t message,uint32_t key)
{return restore_window_handle_debug_key(message,key);}
