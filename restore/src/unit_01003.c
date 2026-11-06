#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define building_bound (*(int32_t *)0x0078d790u)
#define building_active (*(uint8_t *)0x007999d8u)
#define building_kind (*(int16_t *)0x007999e8u)
#define building_dock (*(uint8_t *)0x00799aa2u)
#define dock_clear_links (*(uint32_t *)0x00eab124u)
#define dock_used (*(uint32_t *)0x00eaaf68u)
#define dock_links (*(uint32_t *)0x00eab044u)

extern uint8_t FUN_0046efd0(int32_t building);

void FUN_0046f030(void)
{
    uint32_t *clear = &dock_clear_links;
    int32_t initial_bound;
    do { *clear = 0; initial_bound = building_bound; clear += 56; }
    while ((uintptr_t)clear < 0xeadc04u);
    int32_t initial_scan_bound = building_bound;
    for (int32_t building = 1; building <= initial_scan_bound; ++building) {
        uint32_t offset = (uint32_t)building * 264u;
        uint8_t group = (&building_dock)[offset];
        if ((&building_active)[offset] != 0 && (&building_kind)[(uint32_t)building * 132u] == 0x88 && group != 0)
            (&dock_used)[(uint32_t)group * 56u] = 1;
    }
    if (initial_scan_bound > 0 && initial_bound > 0) {
        for (int32_t building = 1; building <= building_bound; ++building) {
            uint32_t offset = (uint32_t)building * 264u;
            if ((&building_active)[offset] != 0 && (&building_kind)[(uint32_t)building * 132u] == 0x88) {
                uint8_t group = (&building_dock)[offset];
                if (group == 0) (&building_dock)[offset] = FUN_0046efd0(building);
                else if ((&dock_links)[(uint32_t)group * 56u] == 0)
                    (&dock_links)[(uint32_t)group * 56u] = (uint32_t)building;
                else (&building_dock)[offset] = FUN_0046efd0(building);
            }
        }
    }
}
