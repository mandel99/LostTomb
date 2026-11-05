#include "actor_value_contracts.h"
#include "animation_state.h"

/* 4E5B20: only AL is a logical Boolean. Add the child's AL to a
   freshly reloaded progress BYTE, wrap, then apply the unsigned cap. */
uint8_t restore_obelisk_advance_progress(int32_t building)
{
 uint32_t record=restore_monument_worker_building(building);
 int32_t x,y;
 uint8_t increment,progress;
 if(MW_B(record+85u)==100u)return 1;
 increment=(uint8_t)FUN_004e5ae0(building);
 progress=(uint8_t)(MW_B(record+85u)+increment);
 MW_B(record+85u)=progress;
 if(progress>100u)MW_B(record+85u)=100;
 y=MW_H(record+8u);x=MW_H(record+6u);
 restore_obelisk_refresh_progress(building,x,y);
 return (uint8_t)(MW_B(record+85u)==100u);
}
