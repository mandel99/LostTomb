#ifndef PHARAOH_MAP_IMAGE_CONTEXT_H
#define PHARAOH_MAP_IMAGE_CONTEXT_H

#include <stdint.h>
#include <stddef.h>

typedef struct PharaohMapImageContextRecord {
    uint8_t neighbors[8]; /* 0/1 requirement;2 is wildcard */
    uint8_t orientation_offsets[4];
    uint8_t value_0c;
    uint8_t frame_count;
    union { uint8_t active; uint8_t current_frame; };
    uint8_t reserved_0f;
} PharaohMapImageContextRecord;

_Static_assert(sizeof(PharaohMapImageContextRecord) == 16,
    "native map image-context record size changed");
_Static_assert(offsetof(PharaohMapImageContextRecord,orientation_offsets)==8 &&
    offsetof(PharaohMapImageContextRecord,frame_count)==13 &&
    offsetof(PharaohMapImageContextRecord,current_frame)==14,
    "native image-context byte fields");

typedef struct PharaohMapImageSelection {
    uint32_t frame, record_value, orientation_offset;
} PharaohMapImageSelection;

typedef struct PharaohMapImageContext {
    const uint32_t *adjacent; /* eight native DWORD slots */
    int32_t orientation;
    PharaohMapImageSelection *selection;
} PharaohMapImageContext;

/* Native00484BE0 and00484D70. The grouped table contains8*count records. */
int32_t pharaoh_map_image_context_match(const PharaohMapImageContext *c,
    PharaohMapImageContextRecord *records, int32_t count,
    int32_t animation, uint8_t reverse);
int32_t pharaoh_map_image_context_match_grouped(const PharaohMapImageContext *c,
    PharaohMapImageContextRecord *records, int32_t count);

void pharaoh_map_image_context_reset(
    PharaohMapImageContextRecord *records, int32_t count);

#endif
