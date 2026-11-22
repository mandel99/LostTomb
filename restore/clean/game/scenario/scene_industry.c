#include "pharaoh/scenario/scene_industry.h"

_Static_assert(offsetof(PharaohBuildingTypeRecord, farm_progress) == 0x54 &&
    sizeof(PharaohBuildingTypeRecord) == 0x108, "native farm progress WORD");

/* Native00489840: FLD exact10.0;__ftol gives10;IMUL AX,WORD[+54];store AX.
 * Reuse SaveMethods C integer multiply, retaining exactly the low16 bits. */
void pharaoh_game_file_upgrade_farm_progress(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT])
{
    for (int32_t id = 1; id < PHARAOH_BUILDING_RECORD_COUNT; ++id)
        if ((uint8_t)pharaoh_building_is_farm(buildings, id) != 0)
            buildings[id].farm_progress = (int16_t)(uint16_t)
                ((int32_t)buildings[id].farm_progress * 10);
}

#define TYPE_IN_CASES(name, cases) \
int32_t name(int32_t type) { switch (type) { cases return 1; default: return 0; } }
TYPE_IN_CASES(pharaoh_building_type_is_00487f80_group,
    case 100: case 101: case 102: case 103: case 104: case 105: case 196: case 224:)

int32_t pharaoh_building_is_farm(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id)
{
    return pharaoh_building_type_is_00487f80_group(
        buildings[building_id].building_type);
}
