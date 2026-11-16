#ifndef PHARAOH_RENDERING_PRIMITIVES_H
#define PHARAOH_RENDERING_PRIMITIVES_H

#include <stdint.h>

typedef void (*PharaohPrimitivePixelWriter)(void *user_data, int32_t x,
    int32_t y, uint16_t color);

typedef struct PharaohGraphicsPrimitives {
    int32_t display_width;
    int32_t screen_height;
    int32_t clip_x0, clip_x1, clip_y0, clip_y1;
    int32_t line_error, line_end_x, line_end_y;
    int32_t line_start_x, line_start_y, line_dx, line_dy;
    int32_t line_x_direction, line_y_direction;
    int32_t dash_blink_frame, dash_step, dash_color, dash_base_color;
    uint16_t *rgb565_pixels;
    int32_t rgb565_pitch_bytes;
    PharaohPrimitivePixelWriter write_pixel;
    void *user_data;
} PharaohGraphicsPrimitives;

enum {
    PHARAOH_PRIMITIVE_DASH_LENGTH = 4,
    PHARAOH_PRIMITIVE_DASH_BLINK_FRAMES = 20,
    PHARAOH_PRIMITIVE_DASH_BOTTOM_MARGIN = 32,
    PHARAOH_PRIMITIVE_COLOR_BLACK = 0,
    PHARAOH_PRIMITIVE_COLOR_GREY = 0x8410,
    PHARAOH_PRIMITIVE_COLOR_RED = 0xf800,
    PHARAOH_PRIMITIVE_COLOR_WHITE = 0xffff
};

void pharaoh_graphics_draw_line_setup(PharaohGraphicsPrimitives *,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void pharaoh_graphics_draw_line_step(PharaohGraphicsPrimitives *, int32_t walk_y);
void pharaoh_graphics_draw_line(PharaohGraphicsPrimitives *, int32_t x1,
    int32_t y1, int32_t x2, int32_t y2, int32_t color);
void pharaoh_graphics_draw_rect(PharaohGraphicsPrimitives *, int32_t x,
    int32_t y, int32_t width, int32_t height, int32_t color);
void pharaoh_graphics_draw_dashed_rect(PharaohGraphicsPrimitives *, int32_t x,
    int32_t y, int32_t width, int32_t height);
void pharaoh_graphics_dash_blink(PharaohGraphicsPrimitives *);
void pharaoh_graphics_dash_step(PharaohGraphicsPrimitives *);
void pharaoh_graphics_draw_inset_rect(PharaohGraphicsPrimitives *, int32_t x,
    int32_t y, int32_t width, int32_t height);
void pharaoh_graphics_draw_raised_rect(PharaohGraphicsPrimitives *, int32_t x,
    int32_t y, int32_t width, int32_t height);
void pharaoh_graphics_fill_rect(PharaohGraphicsPrimitives *, int32_t x,
    int32_t y, int32_t width, int32_t height, int32_t color);
int32_t pharaoh_graphics_point_in_clip(const PharaohGraphicsPrimitives *,
    int32_t x, int32_t y);

#endif
