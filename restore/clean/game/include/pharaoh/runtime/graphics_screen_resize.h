#ifndef PHARAOH_RUNTIME_GRAPHICS_SCREEN_RESIZE_H
#define PHARAOH_RUNTIME_GRAPHICS_SCREEN_RESIZE_H

#include <stdint.h>

#include "pharaoh/assets/sprite_pixel_loading.h"
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/runtime/graphics_startup.h"
#include "pharaoh/ui/sidebar_layout.h"

typedef struct PharaohGraphicsScreenMemoryOperations {
    void *user_data;
    void *(*allocate)(void *user_data, uint32_t byte_count);
    void (*release)(void *user_data, void *allocation);
} PharaohGraphicsScreenMemoryOperations;

typedef struct PharaohGraphicsScreenResizeContext {
    PharaohGraphicsStartupState *graphics_state;
    PharaohSidebarClipPositions *sidebar_positions;
    const PharaohSpritePixelLoadContext *sprite_pixel_context;
    uint8_t **sprite_pixels;
    uint8_t **software_framebuffer;
    PharaohCityViewContext *city_view;
    const PharaohGraphicsScreenMemoryOperations *memory_operations;
    int32_t viewport_without_sidebar;
} PharaohGraphicsScreenResizeContext;

void pharaoh_graphics_screen_resize(
    const PharaohGraphicsScreenResizeContext *context);

#endif
