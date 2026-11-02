#include "production_updates.h"
#include "menu_building_kind.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_00459240(int32_t building)
{
    uint32_t record=0x7999d8u+(uint32_t)building*264u,resource;
    /* The native overlapping >=106/<=114 guards always clear BL before
     * both switch exits converge on this resource-permission lookup. */
    (void)FUN_00488030((uint32_t)building);
    resource=B(record+67u);
    return H(0xea4126u+U(0xeaac28u)*0x49d8u+resource*2u)!=0;
}
