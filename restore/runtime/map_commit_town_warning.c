#include "map_commit_town_warning.h"
#include "menu_actor_message.h"
#include "pending_message.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint8_t restore_town_warning_eligible(const RestoreTownRecord *record)
{
 uint32_t stack[16];struct restore_actor_message_frame f;unsigned i;
 for(i=0;i<8;++i)f.r[i]=0;
 f.r[0]=f.r[1]=(uint32_t)(uintptr_t)record;
 f.r[4]=(uint32_t)(uintptr_t)(stack+8);stack[8]=0;
 f.flags=2;f.boundary=restore_actor_message_dispatch;
 restore_actor_message_C_0044d700(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+9)||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 return (uint8_t)f.r[0];
}
void FUN_004e2e20(int32_t tool)
{
 RestoreTownRecord *record;uint32_t player;
 if(U(0x605a3cu)==1)return;
 if(!((tool>=65&&tool<=69)||(tool>=60&&tool<=64)||(tool>=140&&tool<=149))){
  /* Exact native BYTE mapper197 entries and its two-target table reduce to
   * these twenty additional tools; all other mapper entries return. */
  switch(tool){
  case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 55:
  case 77:case 78:case 79:case 81:case 86:case 138:case 167:
  case 184:case 187:case 188:case 189:case 226:break;
  default:return;
  }
 }
 record=restore_town_next(RESTORE_TOWNS,NULL,0);
 while(record&&!restore_town_warning_eligible(record))
  record=restore_town_next(RESTORE_TOWNS,(const int16_t *)record,0);
 if(!record)return;
 if(U(0xbcb9b4u))return;
 player=U(0xeaac28u)*0x49d8u;
 if(B(0xea5ed8u+player))return;
 FUN_004e2970(104);
}
