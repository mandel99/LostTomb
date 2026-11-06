#include "final_state_actions.h"
extern void FUN_0046e1c0(void);
extern void FUN_0046e970(uint32_t parameter);
extern void FUN_0046ef50(void);
extern void FUN_0046e7c0(void);
extern void FUN_0046e1e0(void);
void restore_map_run_refresh(uint32_t parameter)
{
 uint32_t mode;
 FUN_0046e1c0();FUN_0046e970(parameter);FUN_0046ef50();FUN_0046e7c0();
 mode=VIEW_U(0x78d7c0u);
 if (mode==0 || mode==3 || mode==6 || mode==9) FUN_0046e1e0();
}
void FUN_0046e180(uint32_t parameter){restore_map_run_refresh(parameter);}
