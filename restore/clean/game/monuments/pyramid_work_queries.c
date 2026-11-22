#include "pharaoh/monuments/progress_reports.h"

uint8_t pharaoh_pyramid_piece_has_pending_work(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const uint8_t *style_one_three_column_rows)
{
    const PharaohBuildingTypeRecord *building = &buildings[building_id];
    const uint32_t row = building->pyramid_work_row;
    const uint32_t height = (uint8_t)building->monument_stage;
    uint32_t columns;
    if (row >= height) return 0;
    if (pyramid_style == 1u) {
        columns = style_one_three_column_rows[
            (uint32_t)building->pyramid_size_variant * 6u + row] != 0u ?
            3u : 6u;
    } else if (pyramid_style == 5u && building->subtype == 0) {
        columns = 1u;
    } else {
        columns = 6u;
    }
    return (uint8_t)(building->pyramid_work_column != columns ||
        row + 1u != height);
}

int32_t pharaoh_pyramid_available_worker_slot(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const int32_t *descriptor_counts,
    const PharaohPyramidWorkDescriptor *descriptors_by_variant)
{
    const PharaohBuildingTypeRecord *building = &buildings[building_id];
    const uint32_t variant = building->pyramid_size_variant;
    const uint8_t *stage_bytes = (const uint8_t *)&building->monument_stage;
    int32_t index;
    int32_t position;
    if (pyramid_style == 5u) return 0;
    if (building->pyramid_work_phase == 1u) {
        const int32_t root_id = building_id - building->subtype;
        if (buildings[root_id].pyramid_work_phase == 1u) return 0;
    }
    position = (int32_t)building->pyramid_work_column +
        (int32_t)building->pyramid_work_row * 6;
    for (index = 0; index < descriptor_counts[variant]; ++index) {
        const PharaohPyramidWorkDescriptor *descriptor =
            &descriptors_by_variant[variant * 48u + (uint32_t)index];
        if (descriptor->part != building->subtype) continue;
        if (descriptor->first_worker_threshold > 0 &&
            descriptor->first_worker_threshold <= position &&
            stage_bytes[1] == 0u) return 1;
        if (descriptor->second_worker_threshold <= 0 ||
            position < descriptor->second_worker_threshold ||
            stage_bytes[2] != 0u) return 0;
        return 2;
    }
    return 0;
}

void pharaoh_pyramid_collect_work_pieces(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, uint32_t pyramid_style,
    const int32_t *regular_part_counts,
    const int32_t *style_five_part_counts,
    int32_t *frontier_parts, int32_t *frontier_count)
{
    const uint32_t variant = buildings[building_id].pyramid_size_variant;
    const int32_t root = building_id - buildings[building_id].subtype;
    const int32_t part_count = pyramid_style == 5u ?
        style_five_part_counts[variant] : regular_part_counts[variant];
    int32_t best = 0;
    int32_t current;
    *frontier_count = 0;
    for (current = root; current < root + part_count; ++current) {
        const PharaohBuildingTypeRecord *candidate = &buildings[current];
        const uint32_t phase = candidate->pyramid_work_phase;
        const uint32_t column = candidate->pyramid_work_column;
        const uint32_t row = candidate->pyramid_work_row;
        if (phase != 0u &&
            (column == 0u || pyramid_style == 3u || pyramid_style == 5u))
            continue;
        if (best != 0) {
            const PharaohBuildingTypeRecord *previous = &buildings[best];
            const uint32_t old_phase = previous->pyramid_work_phase;
            const uint32_t old_column = previous->pyramid_work_column;
            const uint32_t old_row = previous->pyramid_work_row;
            if (phase != old_phase) {
                if (phase != 0u) continue;
            } else if (phase != 0u) {
                if (row < old_row || (row == old_row && column < old_column))
                    continue;
                if (row == old_row && column == old_column) {
                    frontier_parts[(*frontier_count)++] = current;
                    continue;
                }
            } else {
                if (row > old_row || (row == old_row && column > old_column))
                    continue;
                if (row == old_row && column == old_column) {
                    frontier_parts[(*frontier_count)++] = current;
                    continue;
                }
            }
        }
        *frontier_count = 0;
        best = current;
        frontier_parts[(*frontier_count)++] = current;
    }
}

uint8_t pharaoh_pyramid_linked_worker_active(
    const PharaohWalkerQueryRecord *walkers,
    int32_t walker_id, int32_t owner_id)
{
    const PharaohWalkerQueryRecord *walker = &walkers[walker_id];
    uint32_t state;
    if (walker->state == 0 || walker->type != 85) return 0;
    state = walker->action_state;
    if (state == 6 || state == 7) state = walker->action_state_before_6_7;
    switch (state) {
    case 15: case 16: case 17: case 30:
    case 18: case 19: case 29: case 20:
        return walker->leading_walker_id == owner_id;
    default:
        return 0;
    }
}
