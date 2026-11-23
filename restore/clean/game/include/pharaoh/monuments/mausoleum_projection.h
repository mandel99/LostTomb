#ifndef PHARAOH_MONUMENTS_MAUSOLEUM_PROJECTION_H
#define PHARAOH_MONUMENTS_MAUSOLEUM_PROJECTION_H

#include <stdint.h>

enum {
    PHARAOH_MAUSOLEUM_PART_COUNT = 116,
    PHARAOH_MAUSOLEUM_VIEW_TOP = 0,
    PHARAOH_MAUSOLEUM_VIEW_RIGHT = 2,
    PHARAOH_MAUSOLEUM_VIEW_BOTTOM = 4,
    PHARAOH_MAUSOLEUM_VIEW_LEFT = 6
};

typedef struct PharaohMausoleumPartDescriptor {
    int32_t dx;
    int32_t dy;
    int32_t size;
    int32_t kind;
    int32_t grid_delta;
    int32_t view_x[4];
    int32_t view_y[4];
} PharaohMausoleumPartDescriptor;

_Static_assert(sizeof(PharaohMausoleumPartDescriptor) == 52,
    "native mausoleum descriptor is thirteen 32-bit words");

typedef struct PharaohMausoleumProjectionContext {
    const PharaohMausoleumPartDescriptor *initial_parts;
    PharaohMausoleumPartDescriptor *projected_parts;
    uint32_t projection_scale_x;
    uint32_t projection_scale_y;
} PharaohMausoleumProjectionContext;

typedef struct PharaohMausoleumWorkTileRecord {
    uint8_t unknown_00[2];
    int16_t x;
    int16_t y;
    uint8_t unknown_06[0x102];
} PharaohMausoleumWorkTileRecord;

_Static_assert(sizeof(PharaohMausoleumWorkTileRecord) == 0x108,
    "native mausoleum work-tile record size changed");

/* Native direct consumers either test AL or discard the result. */
uint8_t pharaoh_mausoleum_get_work_tile(
    const PharaohMausoleumWorkTileRecord *records, int32_t index,
    int32_t *x, int32_t *y);

/* Original address 0x004DED50. */
void pharaoh_mausoleum_part_describe(
    const PharaohMausoleumProjectionContext *context,
    PharaohMausoleumPartDescriptor *destination, int32_t part,
    int32_t view, int32_t orientation);

/* Original address 0x004DEE10. */
void pharaoh_mausoleum_projection_initialize(
    PharaohMausoleumProjectionContext *context);

#endif
