#ifndef PHARAOH_UI_PANEL_FRAME_H
#define PHARAOH_UI_PANEL_FRAME_H

#include <stdint.h>

typedef struct PharaohPanelFrameOperations {
    void *user_data;
    void (*draw_sprite)(void *user_data, int32_t image, int32_t x, int32_t y);
    int32_t panel_image_base;
    int32_t inner_panel_image_base;
} PharaohPanelFrameOperations;

/* Native tiled frames 0x00536820 and 0x00536A30. */
void pharaoh_panel_frame_draw(const PharaohPanelFrameOperations *operations,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks);
void pharaoh_inner_panel_frame_draw(
    const PharaohPanelFrameOperations *operations,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks);

#endif
