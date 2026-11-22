#ifndef PHARAOH_MONUMENTS_PYRAMID_TILE_QUERY_H
#define PHARAOH_MONUMENTS_PYRAMID_TILE_QUERY_H

#include "pharaoh/building/type_queries.h"

typedef struct PharaohPyramidSpecialStageEntry {
    int32_t part;
    int32_t tile_offset;
} PharaohPyramidSpecialStageEntry;

typedef struct PharaohPyramidSpecialStageTable {
    const PharaohPyramidSpecialStageEntry *entries;
    int32_t count;
} PharaohPyramidSpecialStageTable;

int32_t pharaoh_pyramid_effective_tile_stage(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const int16_t *building_id_grid, uint32_t cell,
    uint32_t pyramid_style,
    const PharaohPyramidSpecialStageTable *regular_tables,
    const PharaohPyramidSpecialStageTable *style_five_tables);

bool pharaoh_pyramid_has_complex_temple(int32_t pyramid_style,
    const int32_t *complex_temple_by_building_type, int32_t building_type);

#endif
