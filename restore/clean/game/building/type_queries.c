#include "pharaoh/building/type_queries.h"

int32_t pharaoh_building_group_index(
    const PharaohBuildingTypeRecord *buildings, int32_t id)
{
    switch (buildings[id].building_type) {
    case 0x3c: case 0x41: return 0;
    case 0x3d: case 0x42: return 1;
    case 0x3e: case 0x43: return 2;
    case 0x3f: case 0x44: return 3;
    case 0x40: case 0x45: return 4;
    default: return -1;
    }
}
