#ifndef PHARAOH_UI_BUTTON_BORDER_H
#define PHARAOH_UI_BUTTON_BORDER_H

#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/ui/panel_frame.h"
#include "pharaoh/ui/button_layout.h"

typedef struct PharaohButtonBorderContext {
    PharaohImageDrawContext *drawing;
    const int32_t *image_base;
    int32_t *tile_x_00d3b5e0, *tile_y_00d3b5dc;
} PharaohButtonBorderContext;

/* Complete native 0x00536C30. style has no effect in this executable. */
void pharaoh_button_border_draw(const PharaohButtonBorderContext *,
    int32_t style, int32_t x, int32_t y, int32_t width, int32_t height,
    uint32_t flags);
/* Complete native 0x004D6050: draw a six-by-six panel at the origin. */
void pharaoh_ui_layout_clear_highlight(const PharaohPanelFrameNativeContext *);

typedef struct PharaohLayoutHighlightDrawing {
    PharaohPanelFrameNativeContext panel;
    PharaohButtonBorderContext border;
} PharaohLayoutHighlightDrawing;

/* Binds actual C panel, border and image renderers. Drawing and all pointed
   state must outlive the highlight state. */
void pharaoh_ui_layout_bind_highlight(PharaohLayoutHighlightState *,
    PharaohLayoutHighlightDrawing *, PharaohImageDrawContext *,
    const int32_t *panel_image_base, const int32_t *border_image_base,
    uint32_t *current_image, int32_t *tile_x, int32_t *tile_y);

#endif
