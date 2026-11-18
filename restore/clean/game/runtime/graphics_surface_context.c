#include "pharaoh/runtime/graphics_startup.h"

enum {
    SURFACE_CONTEXT_PIXEL_FORMAT_RGB565 = 1,
    SURFACE_CONTEXT_PIXEL_FORMAT_RGB555 = 0,
    SURFACE_LOCK_WAIT = 0x01,
    RGB565_RED_MASK = 0xf800,
    RGB565_GREEN_MASK = 0x07e0,
    RGB565_BLUE_MASK = 0x001f,
    RGB555_RED_MASK = 0x7c00,
    RGB555_GREEN_MASK = 0x03e0,
    RGB555_BLUE_MASK = 0x001f
};

static const char front_surface_pointer_format[] = "Front Surface Ptr: %x\n";
static const char back_surface_pointer_format[] = "Back Surface Ptr: %x\n";
static const char foreground_lock_error[] = "Unable to lock fg surface\n";
static const char background_lock_error[] = "Unable to lock bg surface\n";

int32_t pharaoh_graphics_surface_context_initialize(
    PharaohGraphicsSurfaceContext *context,
    void *front_surface, void *back_surface,
    const PharaohGraphicsStartupOperations *operations)
{
    PharaohDirectDrawSurfaceLockResult front_lock = { 0 };
    PharaohDirectDrawSurfaceLockResult back_lock = { 0 };
    int32_t result;

    context->front_surface = front_surface;
    context->back_surface = back_surface;

    result = operations->lock_surface(operations->user_data,
        context->front_surface, SURFACE_LOCK_WAIT, &front_lock);
    if (result == 0) {
        context->front_pixels = front_lock.pixels;
        operations->unlock_surface(operations->user_data,
            context->front_surface);
        operations->output_surface_debug(operations->user_data,
            front_surface_pointer_format, context->front_pixels);
    } else {
        operations->output_surface_debug(operations->user_data,
            foreground_lock_error, NULL);
    }

    result = operations->lock_surface(operations->user_data,
        context->back_surface, SURFACE_LOCK_WAIT, &back_lock);
    if (result == 0) {
        context->back_pixels = back_lock.pixels;
        operations->unlock_surface(operations->user_data,
            context->back_surface);
        operations->output_surface_debug(operations->user_data,
            back_surface_pointer_format, context->back_pixels);
    } else {
        operations->output_surface_debug(operations->user_data,
            background_lock_error, NULL);
    }

    context->width = (int32_t)back_lock.width;
    context->height = (int32_t)back_lock.height;

    if (back_lock.pixel_masks.red == RGB565_RED_MASK &&
        back_lock.pixel_masks.green == RGB565_GREEN_MASK &&
        back_lock.pixel_masks.blue == RGB565_BLUE_MASK) {
        context->pixel_format = SURFACE_CONTEXT_PIXEL_FORMAT_RGB565;
        return 1;
    }
    if (back_lock.pixel_masks.red == RGB555_RED_MASK &&
        back_lock.pixel_masks.green == RGB555_GREEN_MASK &&
        back_lock.pixel_masks.blue == RGB555_BLUE_MASK) {
        context->pixel_format = SURFACE_CONTEXT_PIXEL_FORMAT_RGB555;
        return 1;
    }

    context->pixel_format = -1;
    return 0;
}
