#include "final_state_actions.h"
extern void FUN_0047d870(uint32_t first);
extern void FUN_0047d720(void);
extern void FUN_004dea50(void);
extern void FUN_0046d840(void);
extern void FUN_0047fb70(void);
extern void FUN_005178e0(void);
extern void FUN_00497b90(void);
extern void FUN_004ddf90(void);
void restore_world_initialize_subsystems(void)
{
 FUN_0047d870(1);VIEW_U(0xbc9694u)=0;
 FUN_0047d720();FUN_004dea50();FUN_0046d840();FUN_0047fb70();
 FUN_005178e0();FUN_00497b90();FUN_004ddf90();
}
void FUN_0047d6e0(void){restore_world_initialize_subsystems();}
