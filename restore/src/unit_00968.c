#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define building_bound (*(int32_t *)0x0078d790u)
#define highest_group (*(int32_t *)0x0078d788u)
#define building_active (*(uint8_t *)0x007999d8u)
#define building_kind (*(int16_t *)0x007999e8u)
#define building_group (*(uint8_t *)0x00799a99u)
#define group_clear_links (*(uint16_t *)0x00790180u)
#define group_used (*(uint8_t *)0x007900c0u)
#define group_links (*(uint16_t *)0x007900bcu)
#define group_scan_flags (*(uint8_t *)0x00790184u)

extern uint8_t FUN_0046b100(int32_t building);

void FUN_0046b320(void)
{
    uint16_t *clear = &group_clear_links;
    int32_t initial_bound;
    do { *clear = 0; initial_bound = building_bound; clear += 98; }
    while ((uintptr_t)clear < 0x7999dcu);
    for (int32_t building = 1; building <= initial_bound; ++building) {
        uint32_t offset = (uint32_t)building * 264u;
        int16_t kind = (&building_kind)[(uint32_t)building * 132u];
        uint8_t group = (&building_group)[offset];
        if ((&building_active)[offset] != 0 &&
            (kind == 0x47 || kind == 0x48 || kind == 0x46 || kind == 0xbf || kind == 0xc1 || kind == 0xbe) && group != 0)
            (&group_used)[(uint32_t)group * 196u] = 1;
    }
    for (int32_t building = 1; building <= building_bound; ++building) {
        uint32_t offset = (uint32_t)building * 264u;
        int16_t kind = (&building_kind)[(uint32_t)building * 132u];
        if ((&building_active)[offset] != 0 &&
            (kind == 0x47 || kind == 0x48 || kind == 0x46 || kind == 0xbf || kind == 0xc1 || kind == 0xbe)) {
            uint8_t group = (&building_group)[offset];
            if (group == 0) (&building_group)[offset] = FUN_0046b100(building);
            else if ((&group_links)[(uint32_t)group * 98u] == 0)
                (&group_links)[(uint32_t)group * 98u] = (uint16_t)building;
            else (&building_group)[offset] = FUN_0046b100(building);
        }
    }
    highest_group = 0;
    int32_t group = 1;
    uint8_t *used = &group_scan_flags;
    do { if (*used != 0) highest_group = group; used += 196; ++group; }
    while ((uintptr_t)used < 0x7999e0u);
}
