#include "pharaoh/monuments/pyramid_tile_query.h"

/* Unchanged canonical native004F3D70 and004ECEA0 query owners. */
int32_t pharaoh_pyramid_effective_tile_stage(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const int16_t *building_id_grid, uint32_t cell,
    uint32_t pyramid_style,
    const PharaohPyramidSpecialStageTable *regular_tables,
    const PharaohPyramidSpecialStageTable *style_five_tables)
{
    const int32_t building_id = building_id_grid[cell];
    const PharaohBuildingTypeRecord *building;
    uint32_t delta;
    uint8_t mask;
    uint32_t stage;
    if (building_id == 0 || buildings[building_id].building_type != 183)
        return -1;
    building = &buildings[building_id];
    delta = cell - (uint32_t)building->grid_offset;
    switch (delta) {
        case 0: mask = 1u; break;
        case 1: mask = 2u; break;
        case 228: mask = 4u; break;
        case 229: mask = 8u; break;
        default: return 2;
    }
    stage = building->pyramid_work_phase;
    if (stage == 3u) {
        const PharaohPyramidSpecialStageTable *tables =
            pyramid_style == 5u ? style_five_tables : regular_tables;
        const PharaohPyramidSpecialStageTable *table =
            &tables[building->pyramid_size_variant];
        int32_t index;
        for (index = 0; index < table->count; ++index) {
            const PharaohPyramidSpecialStageEntry *entry =
                &table->entries[index];
            if (entry->part == building->subtype) {
                return cell == (uint32_t)(building->grid_offset +
                    entry->tile_offset) ? 3 : 2;
            }
        }
        return 2;
    }
    if ((((const uint8_t *)&building->monument_stage)[3] & mask) != 0u)
        return (int32_t)stage;
    --stage;
    return stage == 3u ? 2 : (int32_t)stage;
}

bool pharaoh_pyramid_has_complex_temple(int32_t pyramid_style,
    const int32_t *complex_temple_by_building_type, int32_t building_type)
{
    return pyramid_style != 5 &&
        complex_temple_by_building_type[building_type] != 0;
}
