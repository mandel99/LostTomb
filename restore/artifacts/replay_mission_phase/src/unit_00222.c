#include "replay_mission.h"
extern void FUN_0041aa20(uint32_t first,uint32_t mode,int value);
extern int FUN_00418140(void);
void RM_PUBLIC(FUN_004181e0)(void)
{
    FUN_0041aa20(0,0x35,1000);
    RM_PUT32(0x7027a8,0);RM_PUT32(0xd3b130,0);RM_PUT32(0xd3b138,0);
    RM_PUT32(0xd3b124,FUN_00418140());
}
