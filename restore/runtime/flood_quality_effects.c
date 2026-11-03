#include "flood_settings.h"
extern uint32_t FUN_00562348(void);

void restore_flood_quality_decrease(uintptr_t world)
{
    uint32_t sample=FUN_00562348();
    restore_god_settings_004bd5e0((uint32_t)world+0x32c40u,-(int32_t)((sample%8u+3u)*10u));
}
void restore_flood_quality_increase(uintptr_t world)
{
    uint32_t sample=FUN_00562348();
    restore_god_settings_004bd5e0((uint32_t)world+0x32c40u,(int32_t)((sample%8u+3u)*10u));
}
