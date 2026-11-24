#include "pharaoh/rendering/minimap_image.h"

/* Whole21-instruction0053F4A0, reused mixed Unit02526 RestoreMinimapCell.
 * Invalid requested IDs leave all drawing globals untouched. A valid ID
 * beyond the loaded count falls back to record0 before the source test.
 * Pointer positivity is the original signed Win32 address test; no invented
 * image metadata, decompression, color key or clip test belongs here. */
void pharaoh_minimap_draw_image(PharaohMinimapImage *c,int32_t image,int32_t x,int32_t y)
{
    if(image<=0 || image>=25500) return;
    if(image>=*c->image_count) image=0;
    *c->source_cursor=c->image_pixels[image];
    if((intptr_t)*c->source_cursor<=0) return;
    *c->draw_x=x;*c->draw_y=y;
    pharaoh_graphics_copy_pixel_pair_raw(c->target,x,y,
        (const uint16_t *)(const void *)*c->source_cursor);
}
static void image(void *data,int32_t id,int32_t x,int32_t y)
{pharaoh_minimap_draw_image(data,id,x,y);}
void pharaoh_minimap_bind_images(PharaohMinimap *m,PharaohMinimapImage *c)
{m->draw_image=image;m->image_user_data=c;}
