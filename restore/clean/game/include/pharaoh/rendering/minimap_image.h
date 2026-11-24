#ifndef PHARAOH_RENDERING_MINIMAP_IMAGE_H
#define PHARAOH_RENDERING_MINIMAP_IMAGE_H
#include "pharaoh/rendering/minimap.h"

typedef struct PharaohMinimapImage {
    const uint8_t *const *image_pixels;
    const int32_t *image_count;
    const uint8_t **source_cursor;
    int32_t *draw_x,*draw_y;
    PharaohGraphicsPrimitives *target;
} PharaohMinimapImage;

/* Native0053F4A0 is a raw two-pixel copy, not the general SG3 draw path.
 * Source pixel pointers and target must already be loaded in native format. */
void pharaoh_minimap_draw_image(PharaohMinimapImage *,int32_t image,int32_t x,int32_t y);
void pharaoh_minimap_bind_images(PharaohMinimap *,PharaohMinimapImage *);
#endif
