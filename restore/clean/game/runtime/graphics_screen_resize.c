#include "pharaoh/runtime/graphics_screen_resize.h"

enum {
    PHARAOH_SOFTWARE_FRAMEBUFFER_MINIMUM_BYTES = 1200000,
    PHARAOH_SOFTWARE_FRAMEBUFFER_BYTES_PER_PIXEL = 2
};

void pharaoh_graphics_screen_resize(
    const PharaohGraphicsScreenResizeContext *context)
{
    PharaohGraphicsStartupState *const graphics = context->graphics_state;
    uint8_t **const framebuffer = context->software_framebuffer;

    pharaoh_sidebar_init_positions((uint32_t)graphics->render_width,
        context->sidebar_positions);
    pharaoh_sprite_release_pixels(context->sprite_pixel_context,
        context->sprite_pixels, 1);

    if (*framebuffer != NULL) {
        context->memory_operations->release(
            context->memory_operations->user_data, *framebuffer);
    }

    const uint32_t wrapped_byte_count =
        (uint32_t)graphics->render_height *
        (uint32_t)graphics->render_width *
        PHARAOH_SOFTWARE_FRAMEBUFFER_BYTES_PER_PIXEL;
    const uint32_t allocation_bytes =
        (int32_t)wrapped_byte_count >
            PHARAOH_SOFTWARE_FRAMEBUFFER_MINIMUM_BYTES
        ? wrapped_byte_count
        : PHARAOH_SOFTWARE_FRAMEBUFFER_MINIMUM_BYTES;
    *framebuffer = context->memory_operations->allocate(
        context->memory_operations->user_data, allocation_bytes);

    context->city_view->screen_width = graphics->render_width;
    if (context->viewport_without_sidebar == 1) {
        pharaoh_city_view_set_viewport_without_sidebar(context->city_view);
    } else {
        pharaoh_city_view_set_viewport_with_sidebar(context->city_view);
    }
    pharaoh_city_view_calculate_map_bounds(context->city_view);
    pharaoh_city_view_check_camera_boundaries(context->city_view);
}
