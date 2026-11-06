#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define building_bound (*(int32_t *)0x0078d790u)
#define building_active (*(uint8_t *)0x007999d8u)
#define building_kind (*(int16_t *)0x007999e8u)
#define building_group (*(uint8_t *)0x00799aa3u)
#define group_clear_links (*(uint32_t *)0x009783d8u)
#define group_used (*(uint8_t *)0x009783a4u)
#define group_links (*(uint32_t *)0x009783bcu)

extern void FUN_0046a280(int32_t building);

void FUN_0046a320(void)
{
    uint32_t *clear = &group_clear_links;
    do { *clear = 0; clear += 7; } while ((uintptr_t)clear < 0x9784f0u);
    int32_t initial_bound = building_bound;
    for (int32_t building = 1; building <= initial_bound; ++building) {
        uint32_t offset = (uint32_t)building * 264u;
        uint8_t group = (&building_group)[offset];
        if ((&building_active)[offset] != 0 &&
            (&building_kind)[(uint32_t)building * 132u] == 0x4a && group != 0)
            (&group_used)[(uint32_t)group * 28u] = 1;
    }
    for (int32_t building = 1; building <= building_bound; ++building) {
        uint32_t offset = (uint32_t)building * 264u;
        if ((&building_active)[offset] != 0 && (&building_kind)[(uint32_t)building * 132u] == 0x4a) {
            uint8_t group = (&building_group)[offset];
            if (group == 0) FUN_0046a280(building);
            else if ((&group_links)[(uint32_t)group * 7u] == 0)
                (&group_links)[(uint32_t)group * 7u] = (uint32_t)building;
            else FUN_0046a280(building);
        }
    }
}
