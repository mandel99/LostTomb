#include "pharaoh/monuments/mausoleum_projection.h"

#include <string.h>

enum {
    MAUSOLEUM_BOX_WIDTH = 0x16,
    MAUSOLEUM_BOX_DEPTH = 8,
    MAP_GRID_WIDTH = 0xE4
};

static int32_t signed_word(uint32_t value)
{
    int32_t result;
    memcpy(&result, &value, sizeof(result));
    return result;
}

static int32_t grid_delta(int32_t x, int32_t y)
{
    return signed_word((uint32_t)x + (uint32_t)y * MAP_GRID_WIDTH);
}

static void reflect_part_in_box(PharaohMausoleumPartDescriptor *part)
{
    part->dx = signed_word((uint32_t)MAUSOLEUM_BOX_WIDTH -
        (uint32_t)part->dx - (uint32_t)part->size);
    part->dy = signed_word((uint32_t)MAUSOLEUM_BOX_DEPTH -
        (uint32_t)part->dy - (uint32_t)part->size);
}

/* Complete C translation of Pharaoh.exe 0x004E0C50. */
uint8_t pharaoh_mausoleum_get_work_tile(
    const PharaohMausoleumWorkTileRecord *records, int32_t index,
    int32_t *x, int32_t *y)
{
    *x = records[index].x;
    *y = records[index].y;
    return 1;
}

void pharaoh_mausoleum_part_describe(
    const PharaohMausoleumProjectionContext *context,
    PharaohMausoleumPartDescriptor *destination, int32_t part,
    int32_t view, int32_t orientation)
{
    *destination = context->initial_parts[part];

    if (orientation != PHARAOH_MAUSOLEUM_VIEW_TOP &&
        orientation != PHARAOH_MAUSOLEUM_VIEW_BOTTOM) {
        if (view == PHARAOH_MAUSOLEUM_VIEW_BOTTOM ||
            view == PHARAOH_MAUSOLEUM_VIEW_LEFT) {
            reflect_part_in_box(destination);
        }

        {
            const int32_t old_x = destination->dx;
            destination->dx = destination->dy;
            destination->dy = old_x;
        }
        destination->grid_delta = grid_delta(destination->dx,
            destination->dy);
        return;
    }

    if (view == PHARAOH_MAUSOLEUM_VIEW_BOTTOM ||
        view == PHARAOH_MAUSOLEUM_VIEW_RIGHT) {
        reflect_part_in_box(destination);
        destination->grid_delta = grid_delta(destination->dx,
            destination->dy);
    }
}

void pharaoh_mausoleum_projection_initialize(
    PharaohMausoleumProjectionContext *context)
{
    int32_t part;
    const uint32_t horizontal_step = context->projection_scale_x + 1u;
    const uint32_t vertical_scale = context->projection_scale_y;

    for (part = 0; part < PHARAOH_MAUSOLEUM_PART_COUNT; ++part) {
        PharaohMausoleumPartDescriptor view;
        PharaohMausoleumPartDescriptor *destination =
            &context->projected_parts[part];
        uint32_t x;
        uint32_t y;

        pharaoh_mausoleum_part_describe(context, &view, part,
            PHARAOH_MAUSOLEUM_VIEW_TOP, PHARAOH_MAUSOLEUM_VIEW_TOP);
        x = (uint32_t)view.dx;
        y = (uint32_t)view.dy;
        destination->grid_delta = signed_word(x + y * MAP_GRID_WIDTH);
        destination->view_x[0] = signed_word((x - y) * horizontal_step);
        destination->view_y[0] = signed_word((x + y) * vertical_scale);

        pharaoh_mausoleum_part_describe(context, &view, part,
            PHARAOH_MAUSOLEUM_VIEW_RIGHT,
            PHARAOH_MAUSOLEUM_VIEW_RIGHT);
        x = (uint32_t)view.dx;
        y = (uint32_t)view.dy;
        destination->view_x[1] = signed_word((x + y) * horizontal_step);
        destination->view_y[1] = signed_word((y - x) * vertical_scale);

        pharaoh_mausoleum_part_describe(context, &view, part,
            PHARAOH_MAUSOLEUM_VIEW_BOTTOM,
            PHARAOH_MAUSOLEUM_VIEW_BOTTOM);
        x = (uint32_t)view.dx;
        y = (uint32_t)view.dy;
        destination->view_x[2] = signed_word((y - x) * horizontal_step);
        destination->view_y[2] = signed_word(
            0u - (x + y) * vertical_scale);

        pharaoh_mausoleum_part_describe(context, &view, part,
            PHARAOH_MAUSOLEUM_VIEW_LEFT,
            PHARAOH_MAUSOLEUM_VIEW_LEFT);
        x = (uint32_t)view.dx;
        y = (uint32_t)view.dy;
        destination->view_x[3] = signed_word(
            0u - (x + y) * horizontal_step);
        destination->view_y[3] = signed_word((x - y) * vertical_scale);
    }
}
