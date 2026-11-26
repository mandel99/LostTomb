#ifndef PHARAOH_UI_PANEL_BLOCKS_H
#define PHARAOH_UI_PANEL_BLOCKS_H
#include "pharaoh/rendering/image_draw.h"

typedef struct PharaohPanelOpeningFrame {
    int32_t image, x, y;
} PharaohPanelOpeningFrame;
_Static_assert(sizeof(PharaohPanelOpeningFrame) == 12, "native opening frame stride");

typedef struct PharaohPanelBlocksContext {
    const int32_t *animations, *image_base;
    const PharaohPanelOpeningFrame *opening_frames; /* ten native rows005F81E4 */
    int32_t *draw_x, *draw_y;
    uint32_t *current_image;
    PharaohImageDrawContext *drawing;
} PharaohPanelBlocksContext;

/* Complete native00536DC0; opening frames preserve current_image as native. */
void pharaoh_panel_blocks_draw(const PharaohPanelBlocksContext *, int32_t kind,
    int32_t x, int32_t y, int32_t width, int32_t height, int32_t style);
#endif
