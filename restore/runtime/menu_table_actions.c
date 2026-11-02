#include "menu_table_actions.h"
#include "replay_mission.h"
#define ACTION_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define ACTION_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004cb6a0(void);
void FUN_004ea7f0(void)
{
 uint32_t offset=ACTION_U(0xeaac28u)*0x49d8u;
 ACTION_U(0xea5d40u+offset)=0;
 ACTION_U(0xea5d44u+offset)=0;
 ACTION_U(0xea5d48u+offset)=0;
 FUN_004cb6a0();
 ACTION_U(0xbc9694u)=0;
 ACTION_B(0xe38dcau)=1;
}
void FUN_004ea830(void){FUN_00418510();}
void FUN_004ea840(void)
{
 uint32_t address=0xea6018u+ACTION_U(0xeaac28u)*0x49d8u;
 uint32_t value=ACTION_U(address)-1u;
 ACTION_U(0xe39080u)=value;
 if((int32_t)value>1){value=1;ACTION_U(0xe39080u)=value;}
 ACTION_U(address)=value;
 ACTION_B(0xe38ddau)=1;
 ACTION_B(0xe38dcau)=1;
}
