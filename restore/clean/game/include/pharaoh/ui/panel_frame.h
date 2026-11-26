#ifndef PHARAOH_UI_PANEL_FRAME_H
#define PHARAOH_UI_PANEL_FRAME_H

#include <stdint.h>
#include "pharaoh/rendering/image_draw.h"

typedef struct PharaohPanelFrameOperations {
    void *user_data;
    void (*draw_sprite)(void *user_data, int32_t image, int32_t x, int32_t y);
    int32_t panel_image_base;
    int32_t inner_panel_image_base;
} PharaohPanelFrameOperations;

/* Shared sprite-pattern cores. Native owners below also preserve globals. */
void pharaoh_panel_frame_draw(const PharaohPanelFrameOperations *operations,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks);
void pharaoh_inner_panel_frame_draw(
    const PharaohPanelFrameOperations *operations,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks);

typedef struct PharaohPanelFrameNativeContext {
    PharaohImageDrawContext *drawing;
    const int32_t *image_base;
    uint32_t *current_image_00d3b5c0;
    int32_t *tile_x_00d3b5e0, *tile_y_00d3b5dc;
} PharaohPanelFrameNativeContext;

/* Complete native 0x00536820/0x00536A30, with their shared image/glyph
   DWORD and scratch coordinates written before every real image draw. */
void pharaoh_panel_frame_draw_native(const PharaohPanelFrameNativeContext *,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks);
void pharaoh_inner_panel_frame_draw_native(const PharaohPanelFrameNativeContext *,
    int32_t x, int32_t y, int32_t width_blocks, int32_t height_blocks);

void pharaoh_panel_pattern_fill_native(const PharaohPanelFrameNativeContext *,
    int32_t x,int32_t y,int32_t width,int32_t height);
#endif
