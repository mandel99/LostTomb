#include "city_region_children.h"
#include "menu_prepare_integer.h"
#include "god_building_effects.h"
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole00468B20. Read the signed link after the child, using the cached
   264-byte record offset. Preserve the native first iteration and loops. */
void restore_city_mark_linked_fire(uint32_t building)
{
    uint32_t current=FUN_00475b50(building),offset;
    int32_t next;
    for(;;){
        offset=current*264u;
        W(0x799a22u+offset)=1u;
        W(0x799a20u+offset)=1000u;
        restore_god_building_0046d100((int32_t)current,0);
        next=(int32_t)(int16_t)W(0x799a12u+offset);
        if(next<1)return;
        current=(uint32_t)next;
    }
}

void FUN_00468b20(uint32_t building)
{
    restore_city_mark_linked_fire(building);
}
