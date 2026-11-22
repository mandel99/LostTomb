#include "pharaoh/building/type_queries.h"

/* Complete C translation of Pharaoh.exe 0x00475B50. */
int32_t pharaoh_building_follow_link(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    int32_t count = 0;
    do {
        int32_t next = buildings[id].runtime_value_038;
        if (next < 1) return id;
        id = next;
    } while (++count < 0x20d);
    return 0;
}
