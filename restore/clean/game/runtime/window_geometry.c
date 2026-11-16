#include "pharaoh/runtime/window_geometry.h"

enum {
    DISPLAY_MODE_800X600 = 2,
    DISPLAY_MODE_1024X768 = 3,
    DESIGN_WIDTH = 640,
    DESIGN_HEIGHT = 480,
    BYTES_PER_PIXEL = 2,
    INITIAL_MODE_STATUS = 1
};

void pharaoh_initial_window_geometry(int32_t display_mode,
    PharaohInitialWindowGeometry *g)
{
    g->display_mode = display_mode;
    if (display_mode == DISPLAY_MODE_800X600) {
        g->window_width = 800;
        g->window_height = 600;
        g->viewport_width = 800;
    } else if (display_mode == DISPLAY_MODE_1024X768) {
        g->window_width = 1024;
        g->window_height = 768;
        g->viewport_width = 1024;
    } else {
        g->window_width = 640;
        g->window_height = 480;
        g->viewport_width = 640;
    }
    g->ui_origin_x = (g->window_width - DESIGN_WIDTH) / 2;
    g->ui_origin_y = (g->window_height - DESIGN_HEIGHT) / 2;
    g->rgb565_pitch_bytes = g->window_width * BYTES_PER_PIXEL;
    g->framebuffer_bytes = (uint32_t)g->rgb565_pitch_bytes *
        (uint32_t)g->window_height;
    g->viewport_height = g->window_height;
    g->hit_width = g->window_width;
    g->hit_height = g->window_height;
    g->display_mode_status = INITIAL_MODE_STATUS;
}
