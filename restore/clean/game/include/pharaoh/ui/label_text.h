#ifndef PHARAOH_UI_LABEL_TEXT_H
#define PHARAOH_UI_LABEL_TEXT_H

#include <stdint.h>

typedef struct PharaohLabelRenderer {
    int32_t origin_x, origin_y, block_size;
    int32_t opening_animation, growing_animation, blocks_offset;
    int32_t opening_images, opening_images_per_style, pieces_per_style;
    int32_t panel_button_images;
    const int32_t *animations;
    const int32_t *large_frames;
    int32_t frame_fields, frame_x, frame_y, frame_image;
    void (*draw_number)(void *, int32_t, int32_t, const char *, int32_t,
        int32_t, int32_t, int32_t);
    void (*draw_language)(void *, int32_t, int32_t, int32_t, int32_t,
        int32_t, int32_t, int32_t, int32_t);
    void (*draw_image)(void *, int32_t, int32_t, int32_t);
    void *user_data;
} PharaohLabelRenderer;

void pharaoh_text_draw_number_rect(const PharaohLabelRenderer *renderer,
    int32_t value, int32_t prefix, const char *postfix, const int16_t rect[4],
    int32_t font, int32_t color, int32_t dx, int32_t dy);
void pharaoh_text_draw_language_rect(const PharaohLabelRenderer *renderer,
    int32_t group, int32_t index, const int16_t rect[4], int32_t font,
    int32_t color, int32_t dx, int32_t dy, int32_t offset_x,
    int32_t offset_y);
void pharaoh_large_label_draw_animated(const PharaohLabelRenderer *renderer,
    int32_t id, int32_t x, int32_t y, int32_t width_blocks, int32_t type);

#endif
