#include "group_clear.h"
#include "actor_teardown_model.h"
#include "building_cleanup.h"
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
void FUN_004b82e0(int32_t group)
{
 uint32_t row;int32_t actor;
 if(group<=0)return;
 row=0xbcb9c8u+(uint32_t)group*144u;
 if(B(row)==0)return;
 actor=(int32_t)(int16_t)W(row+64u);
 if(actor!=0)FUN_004b4980(actor);
 FUN_0040f8f0((uint32_t *)(uintptr_t)row,144,0);FUN_004b77d0();
}
