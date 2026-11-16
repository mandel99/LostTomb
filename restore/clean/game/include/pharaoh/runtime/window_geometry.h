#ifndef PHARAOH_RUNTIME_WINDOW_GEOMETRY_H
#define PHARAOH_RUNTIME_WINDOW_GEOMETRY_H

#include <stdint.h>

typedef struct PharaohInitialWindowGeometry {
    int32_t display_mode;
    int32_t window_width, window_height;
    int32_t viewport_width, viewport_height;
    int32_t ui_origin_x, ui_origin_y;
    int32_t rgb565_pitch_bytes, hit_width, hit_height;
    uint32_t framebuffer_bytes;
    uint8_t display_mode_status;
} PharaohInitialWindowGeometry;

/* Exact geometry sub-block at the start of original 0x004CF960.
 * Win32 window creation and the rest of that procedure are separate. */
void pharaoh_initial_window_geometry(int32_t settings_display_mode,
    PharaohInitialWindowGeometry *geometry);

#endif
