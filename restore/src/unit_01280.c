#include "production_updates.h"
#include "menu_building_kind.h"
#include "map_commit_population.h"
#include "world_building_draw.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* 488350: genuine capacity calculation and native farm-image refresh.
 * The classifier admits only kinds with a defined farm image selector;
 * the refresh's invalid-kind saved-EBX fallback is unreachable here. */
uint8_t FUN_00488350(int32_t building)
{
    uint32_t record=0x7999d8u+(uint32_t)building*264u;int32_t amount;
    if(!(uint8_t)FUN_00488030((uint32_t)building))return 0;
    if(B(record+173u)){--B(record+173u);return 1;}
    amount=restore_building_capacity_00488140(building,0);
    if(amount){
        W(record+84u)=(uint16_t)(W(record+84u)+(uint16_t)amount);
        if(H(record+84u)>(int32_t)U(0xbca34cu))W(record+84u)=(uint16_t)U(0xbca34cu);
    }
    restore_building_draw_00488c20(0xbca350u,1u,building,H(record+6u),H(record+8u),0);
    return 1;
}
