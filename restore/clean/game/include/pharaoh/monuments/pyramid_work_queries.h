#ifndef PHARAOH_MONUMENTS_PYRAMID_WORK_QUERIES_H
#define PHARAOH_MONUMENTS_PYRAMID_WORK_QUERIES_H

#include "pharaoh/building/type_queries.h"

typedef struct PharaohPyramidWorkDescriptor {
    int32_t part;
    int32_t first_worker_threshold;
    int32_t second_worker_threshold;
    int32_t reserved_0c;
    int32_t reserved_10;
} PharaohPyramidWorkDescriptor;

uint8_t pharaoh_pyramid_piece_has_pending_work(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const uint8_t *style_one_three_column_rows);
int32_t pharaoh_pyramid_available_worker_slot(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const int32_t *descriptor_counts,
    const PharaohPyramidWorkDescriptor *descriptors_by_variant);
void pharaoh_pyramid_collect_work_pieces(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const int32_t *regular_part_counts,
    const int32_t *style_five_part_counts,
    int32_t *frontier_parts, int32_t *frontier_count);

uint8_t pharaoh_pyramid_linked_worker_active(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id, int32_t owner_id);

#endif
