#include "pharaoh/core/record_geometry.h"

void pharaoh_projection_record_copy(
    PharaohProjectionRecord *destination,
    const PharaohProjectionRecord *source)
{
    destination->map_x = source->map_x;
    destination->map_y = source->map_y;
    destination->preserved_word_2 = source->preserved_word_2;
    destination->preserved_word_3 = source->preserved_word_3;
    destination->map_tile_offset = source->map_tile_offset;

    /* Preserve the native interleaved word 5..8 / word 9..12 store order. */
    destination->difference_step = source->difference_step;
    destination->sum_scale = source->sum_scale;
    destination->sum_step = source->sum_step;
    destination->reverse_difference_scale = source->reverse_difference_scale;
    destination->reverse_difference_step = source->reverse_difference_step;
    destination->reverse_sum_scale = source->reverse_sum_scale;
    destination->reverse_sum_step = source->reverse_sum_step;
    destination->difference_scale = source->difference_scale;
}

void pharaoh_projection_record_update(
    PharaohProjectionRecord *record,
    const PharaohProjectionSettings *settings)
{
    const uint32_t x = record->map_x;
    const uint32_t y = record->map_y;
    const uint32_t one_plus_step = settings->step + 1u;
    const uint32_t difference = x - y;
    const uint32_t sum = x + y;
    const uint32_t scaled_sum = sum * settings->scale;
    const uint32_t stepped_sum = sum * one_plus_step;

    /* Unsigned arithmetic intentionally preserves x86 32-bit wraparound. */
    record->map_tile_offset = x + y * 228u;
    record->difference_step = difference * one_plus_step;
    record->sum_scale = scaled_sum;
    record->sum_step = stepped_sum;
    record->difference_scale = difference * settings->scale;
    record->reverse_sum_step = 0u - stepped_sum;
    record->reverse_difference_scale = (y - x) * settings->scale;
    record->reverse_difference_step = (y - x) * one_plus_step;
    record->reverse_sum_scale = 0u - scaled_sum;
}

void pharaoh_projection_records_update(
    PharaohProjectionRecord *records,
    int32_t count,
    const PharaohProjectionSettings *settings)
{
    int32_t index;

    for (index = 0; index < count; ++index) {
        pharaoh_projection_record_update(&records[index], settings);
    }
}
