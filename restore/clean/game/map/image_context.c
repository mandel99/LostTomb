#include "pharaoh/map/image_context.h"

/* Complete C translation of Pharaoh.exe 0x00484EE0. */
void pharaoh_map_image_context_reset(
    PharaohMapImageContextRecord *records, int32_t count)
{
    for (int32_t index = 0; index < count; ++index) {
        records[index].active = 0;
    }
}

static void publish(const PharaohMapImageContext *c,
    const PharaohMapImageContextRecord *record)
{
    c->selection->orientation_offset = record->orientation_offsets[c->orientation / 2];
    c->selection->record_value = record->value_0c;
}

static void cycle(const PharaohMapImageContext *c,
    PharaohMapImageContextRecord *record)
{
    record->current_frame = (uint8_t)(record->current_frame + 1u);
    if (record->current_frame >= record->frame_count) record->current_frame = 0;
    c->selection->frame = record->current_frame;
}

/* Complete typed reuse of Unit01233, native00484BE0. */
int32_t pharaoh_map_image_context_match(const PharaohMapImageContext *c,
    PharaohMapImageContextRecord *records, int32_t count,
    int32_t animation, uint8_t reverse)
{
    if (count <= 0) return 0;
    for (int32_t ordinal = 0; ordinal < count; ++ordinal) {
        PharaohMapImageContextRecord *row = &records[reverse ? count-ordinal-1 : ordinal];
        int32_t i;
        for (i = 0; i < 8; ++i) {
            const uint8_t control = row->neighbors[i];
            if (control != 2 && ((control != 0) != (c->adjacent[i] != 0))) break;
        }
        if (i != 8) continue;
        publish(c, row);
        /* The reverse path has a different animation sentinel in the EXE. */
        if (reverse ? animation == 0 : animation < 0) cycle(c, row);
        /* Possible original bug: a matched zero frame_count with explicit
         * animation traps in IDIV. The invariant is not proven for all tables. */
        else c->selection->frame = (uint32_t)(animation % (int32_t)row->frame_count);
        return ordinal + 1;
    }
    return 0;
}

/* Complete typed reuse of Unit01234, native00484D70. */
int32_t pharaoh_map_image_context_match_grouped(const PharaohMapImageContext *c,
    PharaohMapImageContextRecord *records, int32_t count)
{
    static const uint8_t groups[16] = {0,1,3,2,5,1,4,2,6,7,1,1,5,1,1,1};
    uint32_t mask = 0;
    for (uint32_t side = 0; side < 4; ++side) {
        if (c->adjacent[side*2u+1u] & 2u)
            mask |= (1u << side) | (1u << ((side+1u) & 3u));
        if (c->adjacent[side*2u] & 2u) mask |= 1u << side;
    }
    if (count <= 0) return 0;
    const uint32_t offset = groups[mask] * (uint32_t)count;
    for (int32_t ordinal = 0; ordinal < count; ++ordinal) {
        PharaohMapImageContextRecord *row = &records[offset+(uint32_t)ordinal];
        int32_t i;
        for (i = 0; i < 8; ++i) {
            const uint8_t control = row->neighbors[i];
            if (control != 2 && ((control != 0) != ((c->adjacent[i] & 1u) != 0))) break;
        }
        if (i != 8) continue;
        publish(c, row);
        cycle(c, row);
        return (int32_t)(offset + (uint32_t)ordinal + 1u);
    }
    return 0;
}
