#include "pharaoh/ui/label_text.h"

void pharaoh_text_draw_number_rect(const PharaohLabelRenderer *r,
    int32_t value, int32_t prefix, const char *postfix, const int16_t rect[4],
    int32_t font, int32_t color, int32_t dx, int32_t dy)
{
    r->draw_number(r->user_data, value, prefix, postfix,
        rect[0] + r->origin_x + dx, rect[1] + r->origin_y + dy, font, color);
}

void pharaoh_text_draw_language_rect(const PharaohLabelRenderer *r,
    int32_t group, int32_t index, const int16_t rect[4], int32_t font,
    int32_t color, int32_t dx, int32_t dy, int32_t offset_x, int32_t offset_y)
{
    r->draw_language(r->user_data, group, index,
        rect[0] + r->origin_x + dx, rect[1] + r->origin_y + dy,
        font, color, offset_x, offset_y);
}

static int32_t piece_part(int32_t index, int32_t blocks)
{
    if (index == 0) return 0;
    return index == blocks - 1 ? 2 : 1;
}

void pharaoh_label_draw_animated(const PharaohLabelRenderer *r,
    int32_t id, int32_t x, int32_t y, int32_t width_blocks,
    int32_t force, int32_t type)
{
    if (force != 1) {
        const int32_t animation = r->animations[id];
        if (animation < 20) return;
        const int32_t visible_blocks =
            (int32_t)((uint32_t)animation - UINT32_C(18));
        if (visible_blocks <= width_blocks) {
            const int32_t offset = (int32_t)(
                ((uint32_t)width_blocks - (uint32_t)visible_blocks) *
                UINT32_C(16));
            x = (int32_t)((uint32_t)x + (uint32_t)(offset / 2));
            width_blocks = visible_blocks;
        }
    }

    for (int32_t column = 0; column < width_blocks; ++column) {
        const uint32_t image = (uint32_t)r->panel_button_images +
            (uint32_t)type * UINT32_C(3) +
            (uint32_t)piece_part(column, width_blocks);
        const int32_t draw_x = (int32_t)((uint32_t)x +
            (uint32_t)column * UINT32_C(16));
        r->draw_image(r->user_data, (int32_t)image, draw_x, y);
    }
}

void pharaoh_large_label_draw_animated(const PharaohLabelRenderer *r,
    int32_t id, int32_t x, int32_t y, int32_t width_blocks, int32_t type)
{
    int32_t animation = r->animations[id];
    if (animation < r->opening_animation) return;
    if (animation < r->growing_animation) {
        const int32_t *frame = &r->large_frames[(animation - r->opening_animation) * r->frame_fields];
        int32_t draw_y = frame[r->frame_y] + y;
        int32_t draw_x = ((width_blocks - 2) * r->block_size) / 2 + frame[r->frame_x] + x;
        int32_t image = frame[r->frame_image] + r->opening_images +
            type * r->opening_images_per_style + r->panel_button_images;
        r->draw_image(r->user_data, image, draw_x, draw_y);
    } else {
        int32_t blocks = animation - r->blocks_offset;
        int32_t i, first_image;
        if (blocks > width_blocks) blocks = width_blocks;
        else x += ((width_blocks - blocks) * r->block_size) / 2;
        first_image = r->panel_button_images + type * r->pieces_per_style;
        for (i = 0; i < blocks; ++i)
            r->draw_image(r->user_data, first_image + piece_part(i, blocks),
                x + i * r->block_size, y);
    }
}
