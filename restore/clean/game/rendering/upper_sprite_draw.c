#include "pharaoh/rendering/upper_sprite_draw.h"

#include "pharaoh/rendering/sprite_rle_blit.h"

static int32_t wrap_add(int32_t left, int32_t right)
{
    return (int32_t)((uint32_t)left + (uint32_t)right);
}

static PharaohSpriteRleBlitTarget rle_target(
    const PharaohUpperSpriteDrawContext *context)
{
    const PharaohSpriteClipState *clip = context->clip;
    PharaohSpriteRleBlitTarget target;
    target.surface_pixels = context->surface_pixels;
    target.surface_pitch_bytes = context->surface_pitch_bytes;
    target.x = clip->draw_x;
    target.y = clip->draw_y;
    target.image_width = clip->image_width;
    target.visible_height = clip->visible_height;
    target.source_rows_to_skip = clip->clipped_top;
    target.destination_row_skip_bytes = clip->destination_row_skip;
    target.left_clip_bytes = clip->clipped_left * 2;
    target.right_visible_width = clip->clipped_right;
    target.solid_color = 0;
    return target;
}

/* Complete typed C translation of Pharaoh.exe 0x0053F250. */
int32_t pharaoh_image_draw_isometric_top(
    const PharaohUpperSpriteDrawContext *context,
    int32_t image_id, int32_t x, int32_t y)
{
    const PharaohSpriteImageRecord *image;
    const uint8_t *pixels;
    PharaohSpriteClipState *clip;
    PharaohSpriteRleBlitTarget target;
    int32_t size;

    if (image_id < 1 || image_id >= 25500) return 0;
    image = &context->images[image_id >= context->image_count ? 0 : image_id];
    if (image->has_isometric_top == 0) return 0;
    pixels = context->image_pixels[
        image_id >= context->image_count ? 0 : image_id];
    *context->source_cursor = pixels;
    if (pixels == 0) return 0;
    pixels += image->isometric_top_bytes;
    *context->source_cursor = pixels;

    clip = context->clip;
    clip->image_width = image->width;
    switch (image->width) {
    case 358: size = 6; break;
    case 298: size = 5; break;
    case 238: size = 4; break;
    case 178: size = 3; break;
    case 118: size = 2; break;
    case 58: size = 1; break;
    default: size = 0; break;
    }
    if (size != 0) {
        clip->draw_x = wrap_add(x, -30 * (size - 1));
        clip->draw_y = wrap_add(
            wrap_add(y, -(int32_t)image->height), 30 * size);
        clip->image_height = (int16_t)(image->height - (15 * size + 1));
    }

    pharaoh_graphics_clip_sprite_horizontal(clip);
    pharaoh_graphics_clip_sprite_vertical(clip);
    if (clip->vertical_clip == 5) return 1;

    target = rle_target(context);
    if (clip->horizontal_clip == 1) {
        (void)pharaoh_image_blit_compressed_left(&target, pixels);
    } else if (clip->horizontal_clip == 2) {
        (void)pharaoh_image_blit_compressed_right(&target, pixels);
    } else {
        pharaoh_image_blit_compressed(&target, pixels);
    }

    if (context->tint_mask != 0) {
        if (clip->horizontal_clip == 1) {
            pharaoh_image_blit_compressed_mask_left(
                &target, pixels, context->tint_mask);
        } else if (clip->horizontal_clip == 2) {
            pharaoh_image_blit_compressed_mask_right(
                &target, pixels, context->tint_mask);
        } else {
            pharaoh_image_blit_compressed_mask(
                &target, pixels, context->tint_mask);
        }
    }
    return 1;
}
