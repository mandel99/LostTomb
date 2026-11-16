#include "pharaoh/rendering/primitives.h"

#include <stddef.h>

static int32_t add_coordinate(int32_t left, int32_t right)
{
    return (int32_t)((uint32_t)left + (uint32_t)right);
}

static int32_t subtract_coordinate(int32_t left, int32_t right)
{
    return (int32_t)((uint32_t)left - (uint32_t)right);
}

void pharaoh_graphics_draw_line_setup(PharaohGraphicsPrimitives *c,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    int32_t start_x = x1, start_y = y1, end_x = x2, end_y = y2;
    c->line_y_direction = 1;
    c->line_x_direction = 1;
    c->line_dx = x1 > x2 ? subtract_coordinate(x1, x2) : subtract_coordinate(x2, x1);
    c->line_dy = y1 > y2 ? subtract_coordinate(y1, y2) : subtract_coordinate(y2, y1);
    if ((c->line_dy > c->line_dx && y1 > y2) ||
        (c->line_dy <= c->line_dx && x1 > x2)) {
        start_x = x2; start_y = y2; end_x = x1; end_y = y1;
    }
    c->line_start_y = start_y; c->line_end_y = end_y;
    c->line_start_x = start_x; c->line_end_x = end_x;
    if (end_x < start_x) c->line_x_direction = -1;
    if (end_y < start_y) c->line_y_direction = -1;
}

void pharaoh_graphics_draw_line_step(PharaohGraphicsPrimitives *c, int32_t walk_y)
{
    int32_t add;
    if (walk_y == 0) {
        add = c->line_dy;
        if (c->line_error >= 0) add = subtract_coordinate(c->line_dy, c->line_dx);
    } else {
        add = c->line_dx;
        if (c->line_error >= 0) add = subtract_coordinate(c->line_dx, c->line_dy);
    }
    c->line_error = add_coordinate(c->line_error, add_coordinate(add, add));
}

static void draw_pixel_clipped(PharaohGraphicsPrimitives *c,
    int32_t x, int32_t y, int32_t color)
{
    if (c->write_pixel && x >= 0 && x < c->display_width &&
        y >= 0 && y < c->screen_height)
        c->write_pixel(c->user_data, x, y, (uint16_t)color);
}

void pharaoh_graphics_draw_line(PharaohGraphicsPrimitives *c,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color)
{
    int32_t x, y;
    pharaoh_graphics_draw_line_setup(c, x1, y1, x2, y2);
    if (x1 == x2) {
        for (y = c->line_start_y; y <= c->line_end_y; y = add_coordinate(y, 1))
            draw_pixel_clipped(c, x1, y, color);
        return;
    }
    if (y1 == y2) {
        for (x = c->line_start_x; x <= c->line_end_x; x = add_coordinate(x, 1))
            draw_pixel_clipped(c, x, y1, color);
        return;
    }
    if (c->line_dx < c->line_dy) {
        c->line_error = subtract_coordinate(add_coordinate(c->line_dx, c->line_dx), c->line_dy);
        x = c->line_start_x;
        for (y = c->line_start_y; y <= c->line_end_y; y = add_coordinate(y, 1)) {
            draw_pixel_clipped(c, x, y, color);
            pharaoh_graphics_draw_line_step(c, 1);
            c->line_dy = add_coordinate(c->line_dy, -1);
            if (c->line_error >= 1) {
                c->line_dx = add_coordinate(c->line_dx, -1);
                x = add_coordinate(x, c->line_x_direction);
            }
        }
        return;
    }
    c->line_error = subtract_coordinate(add_coordinate(c->line_dy, c->line_dy), c->line_dx);
    y = c->line_start_y;
    for (x = c->line_start_x; x <= c->line_end_x; x = add_coordinate(x, 1)) {
        draw_pixel_clipped(c, x, y, color);
        pharaoh_graphics_draw_line_step(c, 0);
        c->line_dx = add_coordinate(c->line_dx, -1);
        if (c->line_error >= 1) {
            c->line_dy = add_coordinate(c->line_dy, -1);
            y = add_coordinate(y, c->line_y_direction);
        }
    }
}

static void clamp_rect(const PharaohGraphicsPrimitives *c,
    int32_t *x, int32_t *y, int32_t *width, int32_t *height)
{
    if (*x <= 0) *x = 0;
    if (add_coordinate(*x, *width) >= c->display_width)
        *width = subtract_coordinate(c->display_width, *x);
    if (*y <= 0) *y = 0;
    if (add_coordinate(*y, *height) >= c->screen_height)
        *height = subtract_coordinate(c->screen_height, *y);
}

void pharaoh_graphics_fill_rect(PharaohGraphicsPrimitives *c, int32_t x,
    int32_t y, int32_t width, int32_t height, int32_t color)
{
    int32_t row, bottom;
    if (width <= 0 || height <= 0) return;
    clamp_rect(c, &x, &y, &width, &height);
    bottom = add_coordinate(y, height);
    for (row = y; row < bottom; row = add_coordinate(row, 1))
        pharaoh_graphics_draw_line(c, x, row,
            add_coordinate(add_coordinate(x, width), -1), row, color);
}

void pharaoh_graphics_draw_rect(PharaohGraphicsPrimitives *c, int32_t x,
    int32_t y, int32_t width, int32_t height, int32_t color)
{
    int32_t right, bottom;
    clamp_rect(c, &x, &y, &width, &height);
    right = add_coordinate(add_coordinate(x, width), -1);
    bottom = add_coordinate(add_coordinate(y, height), -1);
    pharaoh_graphics_draw_line(c, x, y, right, y, color);
    pharaoh_graphics_draw_line(c, x, bottom, right, bottom, color);
    pharaoh_graphics_draw_line(c, x, y, x, bottom, color);
    pharaoh_graphics_draw_line(c, right, y, right, bottom, color);
}

static int32_t next_dash_color(int32_t color)
{
    if (color == PHARAOH_PRIMITIVE_COLOR_RED) return PHARAOH_PRIMITIVE_COLOR_BLACK;
    return color != PHARAOH_PRIMITIVE_COLOR_BLACK ?
        PHARAOH_PRIMITIVE_COLOR_RED : PHARAOH_PRIMITIVE_COLOR_WHITE;
}

void pharaoh_graphics_dash_blink(PharaohGraphicsPrimitives *c)
{
    c->dash_blink_frame = add_coordinate(c->dash_blink_frame, 1);
    if (c->dash_blink_frame >= PHARAOH_PRIMITIVE_DASH_BLINK_FRAMES) {
        c->dash_blink_frame = 0;
        c->dash_base_color = next_dash_color(c->dash_base_color);
    }
}

void pharaoh_graphics_dash_step(PharaohGraphicsPrimitives *c)
{
    c->dash_step = add_coordinate(c->dash_step, 1);
    if (c->dash_step >= PHARAOH_PRIMITIVE_DASH_LENGTH) {
        c->dash_step = 0;
        c->dash_color = next_dash_color(c->dash_color);
    }
}

int32_t pharaoh_graphics_point_in_clip(const PharaohGraphicsPrimitives *c,
    int32_t x, int32_t y)
{
    if (x < c->clip_x0 || x > c->clip_x1 || y < c->clip_y0) return 0;
    return y <= c->clip_y1;
}

static void draw_dash_pixel(PharaohGraphicsPrimitives *c, int32_t x, int32_t y)
{
    if (c->rgb565_pixels && c->rgb565_pitch_bytes >= (int32_t)sizeof(uint16_t) &&
        x >= 0 && x < c->display_width && y >= 0 &&
        y < add_coordinate(c->screen_height, -PHARAOH_PRIMITIVE_DASH_BOTTOM_MARGIN)) {
        uint8_t *row = (uint8_t *)c->rgb565_pixels +
            (size_t)y * (size_t)c->rgb565_pitch_bytes;
        uint16_t *pixel = (uint16_t *)(void *)(row + (size_t)x * sizeof(uint16_t));
        *pixel = (uint16_t)c->dash_color;
    }
}

static void draw_dash_if_in_clip(PharaohGraphicsPrimitives *c,
    int32_t test_x, int32_t x, int32_t y)
{
    if (pharaoh_graphics_point_in_clip(c, test_x, y)) draw_dash_pixel(c, x, y);
    pharaoh_graphics_dash_step(c);
}

void pharaoh_graphics_draw_dashed_rect(PharaohGraphicsPrimitives *c,
    int32_t x, int32_t y, int32_t width, int32_t height)
{
    int32_t column, bottom_column, row, x_end, y_end, bottom, right_test_x;
    c->dash_step = 0;
    x_end = add_coordinate(x, width);
    c->dash_color = c->dash_base_color;
    for (column = x; column < x_end; column = add_coordinate(column, 1))
        draw_dash_if_in_clip(c, column, column, y);
    bottom_column = x;
    bottom = add_coordinate(add_coordinate(y, height), -1);
    for (; bottom_column < x_end; bottom_column = add_coordinate(bottom_column, 1))
        draw_dash_if_in_clip(c, bottom_column, bottom_column, bottom);
    y_end = add_coordinate(y, height);
    for (row = y; row < y_end; row = add_coordinate(row, 1))
        draw_dash_if_in_clip(c, x, x, row);
    right_test_x = add_coordinate(add_coordinate(width, bottom_column), -1);
    for (row = y; row < y_end; row = add_coordinate(row, 1))
        draw_dash_if_in_clip(c, right_test_x, add_coordinate(x_end, -1), row);
}

static void draw_frame(PharaohGraphicsPrimitives *c, int32_t x, int32_t y,
    int32_t width, int32_t height, int32_t top_left, int32_t bottom_right)
{
    int32_t right, bottom;
    if (x < 1) x = 0;
    else if (add_coordinate(x, width) >= c->display_width)
        width = subtract_coordinate(c->display_width, x);
    if (y < 1) y = 0;
    else if (add_coordinate(y, height) >= c->screen_height)
        height = subtract_coordinate(c->screen_height, y);
    right = add_coordinate(add_coordinate(x, width), -1);
    pharaoh_graphics_draw_line(c, x, y, right, y, top_left);
    bottom = add_coordinate(add_coordinate(y, height), -1);
    pharaoh_graphics_draw_line(c, right, y, right, bottom, bottom_right);
    pharaoh_graphics_draw_line(c, x, bottom, right, bottom, bottom_right);
    pharaoh_graphics_draw_line(c, x, y, x, bottom, top_left);
}

void pharaoh_graphics_draw_inset_rect(PharaohGraphicsPrimitives *c,
    int32_t x, int32_t y, int32_t width, int32_t height)
{
    draw_frame(c, x, y, width, height,
        PHARAOH_PRIMITIVE_COLOR_GREY, PHARAOH_PRIMITIVE_COLOR_WHITE);
}

void pharaoh_graphics_draw_raised_rect(PharaohGraphicsPrimitives *c,
    int32_t x, int32_t y, int32_t width, int32_t height)
{
    draw_frame(c, x, y, width, height,
        PHARAOH_PRIMITIVE_COLOR_WHITE, PHARAOH_PRIMITIVE_COLOR_GREY);
}
