#ifndef PHARAOH_CORE_RECORD_GEOMETRY_H
#define PHARAOH_CORE_RECORD_GEOMETRY_H

#include <stdint.h>

typedef struct PharaohProjectionRecord {
    uint32_t map_x;
    uint32_t map_y;
    uint32_t preserved_word_2;
    uint32_t preserved_word_3;
    uint32_t map_tile_offset;
    uint32_t difference_step;
    uint32_t sum_step;
    uint32_t reverse_difference_step;
    uint32_t reverse_sum_step;
    uint32_t sum_scale;
    uint32_t reverse_difference_scale;
    uint32_t reverse_sum_scale;
    uint32_t difference_scale;
} PharaohProjectionRecord;

typedef struct PharaohProjectionSettings {
    uint32_t scale;
    uint32_t step;
} PharaohProjectionSettings;

_Static_assert(sizeof(PharaohProjectionRecord) == 52,
    "The native projection record contains thirteen 32-bit words");

/* Original address 0x004E5140. */
void pharaoh_projection_record_copy(
    PharaohProjectionRecord *destination,
    const PharaohProjectionRecord *source);

/* Original address 0x004E5190. */
void pharaoh_projection_record_update(
    PharaohProjectionRecord *record,
    const PharaohProjectionSettings *settings);

/* Original address 0x004E52B0. */
void pharaoh_projection_records_update(
    PharaohProjectionRecord *records,
    int32_t count,
    const PharaohProjectionSettings *settings);

#endif
