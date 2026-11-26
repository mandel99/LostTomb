#include "pharaoh/runtime/city_view.h"

#include <stddef.h>

enum {
    PHARAOH_CITY_INVALID_TILE_OFFSET_BIAS = 0x0fff0000,
    PHARAOH_CITY_VALID_IMAGE_GRID_MINIMUM = 6,
    PHARAOH_CITY_PROJECTION_MAXIMUM_X =
        PHARAOH_CITY_PROJECTION_WIDTH - 1,
    PHARAOH_CITY_PROJECTION_MAXIMUM_Y =
        PHARAOH_CITY_PROJECTION_HEIGHT - 1,
    PHARAOH_CITY_PROJECTION_CENTER_X = 228,
    PHARAOH_CITY_PROJECTION_CENTER_Y = 115
};

typedef struct PharaohProjectionTraversal {
    int32_t projected_x_start;
    int32_t projected_x_row_step;
    int32_t projected_x_column_step;
    int32_t doubled_y_start;
    int32_t doubled_y_row_step;
    int32_t doubled_y_column_step;
} PharaohProjectionTraversal;

static size_t projection_index(int32_t x, int32_t y)
{
    return (size_t)x * PHARAOH_CITY_PROJECTION_HEIGHT + (size_t)y;
}

static int map_grid_is_inside(const PharaohCityMapBounds *bounds,
                              int32_t x, int32_t y)
{
    if (x > bounds->center_x) {
        if (y <= bounds->center_y) {
            return x - y <= bounds->maximum_x - bounds->center_y;
        }
        return x + y <= bounds->maximum_x + 1 + bounds->center_y;
    }
    if (y <= bounds->center_y) {
        return x + y >= bounds->minimum_x + bounds->center_y;
    }
    return x - y >= bounds->center_x - bounds->maximum_y;
}

static PharaohProjectionTraversal projection_traversal(int32_t orientation)
{
    PharaohProjectionTraversal traversal;
    if (orientation == 0) {
        traversal.projected_x_start = 1;
        traversal.projected_x_row_step = 1;
        traversal.projected_x_column_step = 1;
        traversal.doubled_y_start = 230;
        traversal.doubled_y_row_step = -1;
        traversal.doubled_y_column_step = 1;
    } else if (orientation == 2) {
        traversal.projected_x_start = 230;
        traversal.projected_x_row_step = 1;
        traversal.projected_x_column_step = -1;
        traversal.doubled_y_start = 1;
        traversal.doubled_y_row_step = 1;
        traversal.doubled_y_column_step = 1;
    } else if (orientation == 4) {
        traversal.projected_x_start = 455;
        traversal.projected_x_row_step = -1;
        traversal.projected_x_column_step = -1;
        traversal.doubled_y_start = 230;
        traversal.doubled_y_row_step = 1;
        traversal.doubled_y_column_step = -1;
    } else {
        traversal.projected_x_start = 230;
        traversal.projected_x_row_step = -1;
        traversal.projected_x_column_step = 1;
        traversal.doubled_y_start = 455;
        traversal.doubled_y_row_step = -1;
        traversal.doubled_y_column_step = -1;
    }
    return traversal;
}

void pharaoh_city_view_set_viewport(PharaohCityViewState *state,
                                    int32_t map_width, int32_t map_height,
                                    int32_t screen_x, int32_t screen_y,
                                    int32_t width_tiles,
                                    int32_t height_tiles,
                                    int32_t tile_width,
                                    int32_t tile_height)
{
    PharaohCityViewportGeometry *const viewport = &state->viewport;
    viewport->map_width = map_width;
    viewport->map_height = map_height;
    viewport->screen_x = screen_x;
    viewport->screen_y = screen_y;
    viewport->width_tiles = width_tiles;
    viewport->height_tiles = height_tiles;
    viewport->maximum_camera_x = map_width - width_tiles;
    viewport->screen_right =
        (tile_width + 2) * width_tiles - 2 + screen_x;
    viewport->screen_bottom =
        (height_tiles - 1) * (tile_height / 2) + screen_y;
    viewport->tile_width = tile_width;
    viewport->tile_half_width = tile_width / 2;
    viewport->tile_width_and_half = tile_width + tile_width / 2;
    viewport->tile_double_width = tile_width * 2;
    viewport->tile_height = tile_height;
    viewport->tile_half_height = tile_height / 2;
    viewport->tile_height_and_half = tile_height + tile_height / 2;
    viewport->tile_double_height = tile_height * 2;
}

void pharaoh_city_view_projection_reset(PharaohCityViewContext *context)
{
    PharaohCityProjectionLookup *const projection = context->projection;
    const int32_t invalid_tile_offset = context->invalid_tile_offset_seed +
        PHARAOH_CITY_INVALID_TILE_OFFSET_BIAS;
    for (size_t i = 0; i < PHARAOH_CITY_PROJECTION_CELL_COUNT; ++i) {
        projection->cells[i] = invalid_tile_offset;
    }
    projection->projected_minimum_x = PHARAOH_CITY_PROJECTION_MAXIMUM_X;
    projection->projected_minimum_y = PHARAOH_CITY_PROJECTION_MAXIMUM_Y;
    projection->projected_maximum_x = 0;
    projection->projected_maximum_y = 0;
}

void pharaoh_city_view_calculate_lookup(PharaohCityViewContext *context,
                                        int32_t orientation)
{
    PharaohCityProjectionLookup *const projection = context->projection;
    const PharaohProjectionTraversal traversal =
        projection_traversal(orientation);

    pharaoh_city_view_projection_reset(context);
    projection->map_width = PHARAOH_CITY_MAP_WIDTH;
    projection->map_height = PHARAOH_CITY_MAP_HEIGHT;
    projection->map_origin_x = 0;
    projection->map_origin_y = 0;
    projection->orientation = orientation;

    int32_t grid_offset = 0;
    int32_t row_projected_x = traversal.projected_x_start;
    int32_t row_doubled_y = traversal.doubled_y_start;
    for (int32_t row = 0; row < PHARAOH_CITY_MAP_HEIGHT; ++row) {
        int32_t projected_x = row_projected_x;
        int32_t doubled_y = row_doubled_y;
        for (int32_t column = 0; column < PHARAOH_CITY_MAP_WIDTH;
             ++column, ++grid_offset) {
            const int32_t projected_y = doubled_y / 2;
            const int inside_projection = projected_x >= 0 &&
                projected_y >= 0 &&
                projected_x < PHARAOH_CITY_PROJECTION_WIDTH &&
                projected_y < PHARAOH_CITY_PROJECTION_HEIGHT;
            if (context->map_image_grid[grid_offset] <
                PHARAOH_CITY_VALID_IMAGE_GRID_MINIMUM) {
                if (inside_projection) {
                    projection->cells[projection_index(
                        projected_x, projected_y)] =
                        context->invalid_tile_offset_seed +
                        PHARAOH_CITY_INVALID_TILE_OFFSET_BIAS;
                }
            } else {
                if (projected_y <= projection->projected_minimum_y) {
                    projection->projected_minimum_y = projected_y;
                }
                if (projected_x <= projection->projected_minimum_x) {
                    projection->projected_minimum_x = projected_x;
                }
                if (projected_y >= projection->projected_maximum_y) {
                    projection->projected_maximum_y = projected_y;
                }
                if (projected_x >= projection->projected_maximum_x) {
                    projection->projected_maximum_x = projected_x;
                }
                if (inside_projection) {
                    projection->cells[projection_index(
                        projected_x, projected_y)] = grid_offset;
                }
            }
            projected_x += traversal.projected_x_column_step;
            doubled_y += traversal.doubled_y_column_step;
        }
        row_projected_x += traversal.projected_x_row_step;
        row_doubled_y += traversal.doubled_y_row_step;
    }
}

/* Complete native 004F0AB0 view-grid inverse. The original map starts are
 * reset to zero by 004F0400 before this helper can be used. */
void pharaoh_city_view_tile_from_screen(
    const PharaohCityProjectionLookup *p,int32_t x,int32_t y,
    int32_t *row,int32_t *column)
{
    int32_t cx,cy,a,b,d,e;
    switch(p->orientation){
    case 0:cy=230;cx=p->map_origin_y*2+1;a=-1;b=1;d=1;e=1;break;
    case 2:cx=230;cy=p->map_origin_x*2+1;a=1;b=-1;d=1;e=1;break;
    case 4:cy=230;cx=455-p->map_origin_y*2;a=1;b=-1;d=-1;e=-1;break;
    default:cx=230;cy=455-p->map_origin_y*2;a=-1;b=1;d=-1;e=-1;break;
    }
    const int32_t c=((y-cy)*d-(x-cx)*a)/(e*d-a*b);
    *column=c;*row=(x-c*b-cx)/d;
}

/* Complete native 004F0BA0 tile projection over the live viewport lookup. */
void pharaoh_city_view_tile_to_pixel(const PharaohCityViewState *s,
    const PharaohCityProjectionLookup *p,const PharaohCityMapBounds *bounds,
    int32_t x,int32_t y,int32_t *px,int32_t *py)
{
    int32_t value=p->cells[(size_t)s->camera_y*PHARAOH_CITY_PROJECTION_HEIGHT+
        (size_t)s->camera_x],row,column;
    if(value<INT32_C(0x0fff0000)){column=value%PHARAOH_CITY_MAP_WIDTH;row=value/PHARAOH_CITY_MAP_WIDTH;}
    else pharaoh_city_view_tile_from_screen(p,s->camera_y,s->camera_x*2+1,&row,&column);
    y=bounds->minimum_y-row+y;x=bounds->minimum_x-column+x;
    const int32_t half_width=s->viewport.tile_half_width+1;
    switch(p->orientation){
    case 0:*px=(x-y)*half_width+s->viewport.screen_x;*py=(x+y-1)*s->viewport.tile_half_height+s->viewport.screen_y;break;
    case 2:*px=(x+y)*half_width+s->viewport.screen_x;*py=(y-x-1)*s->viewport.tile_half_height+s->viewport.screen_y;break;
    case 4:*px=(y-x)*half_width+s->viewport.screen_x;*py=s->viewport.screen_y-(x+y+1)*s->viewport.tile_half_height;break;
    case 6:*px=s->viewport.screen_x-(x+y)*half_width;*py=(x-y-1)*s->viewport.tile_half_height+s->viewport.screen_y;break;
    default:return;
    }
}

int32_t pharaoh_city_view_get_viewport_width_tiles(
    const PharaohCityViewState *state)
{
    return state->viewport.width_tiles;
}

int32_t pharaoh_city_view_get_viewport_height_tiles(
    const PharaohCityViewState *state)
{
    return state->viewport.height_tiles;
}

int32_t pharaoh_city_view_width_tiles(const PharaohCityViewState *state)
{
    return pharaoh_city_view_get_viewport_width_tiles(state);
}

int32_t pharaoh_city_view_height_tiles(const PharaohCityViewState *state)
{
    return pharaoh_city_view_get_viewport_height_tiles(state);
}

int32_t pharaoh_city_view_grid_offset_is_outside_map(
    const PharaohCityViewContext *context, int32_t grid_offset)
{
    const int32_t x = grid_offset % PHARAOH_CITY_MAP_WIDTH;
    const int32_t y = grid_offset / PHARAOH_CITY_MAP_WIDTH;
    return !map_grid_is_inside(context->map_bounds, x, y);
}

static void set_standard_viewport(PharaohCityViewContext *context,
                                  int32_t width_tiles,
                                  int32_t height_tiles)
{
    pharaoh_city_view_set_viewport(context->state,
        PHARAOH_CITY_MAP_WIDTH, PHARAOH_CITY_MAP_HEIGHT,
        0, PHARAOH_CITY_VIEW_TOP, width_tiles, height_tiles,
        PHARAOH_CITY_TILE_WIDTH, PHARAOH_CITY_TILE_HEIGHT);
}

void pharaoh_city_view_set_viewport_without_sidebar(
    PharaohCityViewContext *context)
{
    if (context->screen_width == 640) {
        set_standard_viewport(context, 10, 31);
    } else if (context->screen_width == 800) {
        set_standard_viewport(context, 12, 39);
    } else if (context->screen_width == 1024) {
        set_standard_viewport(context, 16, 50);
    } else {
        set_standard_viewport(context, 8, 31);
    }
}

void pharaoh_city_view_set_viewport_with_sidebar(
    PharaohCityViewContext *context)
{
    if (context->screen_width == 800) {
        set_standard_viewport(context, 10, 39);
    } else if (context->screen_width == 1024) {
        set_standard_viewport(context, 14, 50);
    } else {
        set_standard_viewport(context, 8, 31);
    }
}

void pharaoh_city_view_set_editor_viewport(PharaohCityViewContext *context)
{
    set_standard_viewport(context, 5, 15);
}

void pharaoh_city_view_calculate_map_bounds(PharaohCityViewContext *context)
{
    PharaohCityViewState *const state = context->state;
    const int32_t *const cells = context->projection->cells;
    int32_t scan = 0;

    while (scan < PHARAOH_CITY_PROJECTION_MAXIMUM_Y &&
           pharaoh_city_view_grid_offset_is_outside_map(context,
               cells[projection_index(PHARAOH_CITY_PROJECTION_CENTER_X,
                                      scan)])) {
        ++scan;
    }
    state->visible_minimum_x = scan + 1;

    scan = PHARAOH_CITY_PROJECTION_MAXIMUM_Y;
    while (scan > 0 &&
           pharaoh_city_view_grid_offset_is_outside_map(context,
               cells[projection_index(PHARAOH_CITY_PROJECTION_CENTER_X,
                                      scan)])) {
        --scan;
    }
    state->visible_maximum_x = scan - 1;

    scan = 0;
    while (scan < PHARAOH_CITY_MAP_HEIGHT &&
           pharaoh_city_view_grid_offset_is_outside_map(context,
               cells[projection_index(scan * 2,
                   PHARAOH_CITY_PROJECTION_CENTER_Y)])) {
        ++scan;
    }
    state->visible_minimum_y = (scan * 2 + 1) & ~1;

    scan = PHARAOH_CITY_MAP_HEIGHT - 1;
    while (scan > 0 &&
           pharaoh_city_view_grid_offset_is_outside_map(context,
               cells[projection_index(scan * 2,
                   PHARAOH_CITY_PROJECTION_CENTER_Y)])) {
        --scan;
    }
    state->visible_maximum_y = (scan * 2 - 1) & ~1;
}

void pharaoh_city_view_check_camera_boundaries(PharaohCityViewContext *context)
{
    PharaohCityViewState *const state = context->state;
    int32_t minimum_x = state->visible_minimum_x;
    int32_t minimum_y = state->visible_minimum_y;
    int32_t maximum_x = state->visible_maximum_x;
    int32_t maximum_y = state->visible_maximum_y;
    int changed = 0;

    if (context->debug_extra_camera_bounds_enabled != 0 &&
        context->event_54_suppress_flood == 1) {
        minimum_x -= 2;
        minimum_y -= 5;
        maximum_x += 3;
        maximum_y += 5;
    }
    if (state->camera_x < minimum_x) {
        state->camera_x = minimum_x;
        changed = 1;
    }
    if (state->camera_y < minimum_y) {
        state->camera_y = minimum_y;
        changed = 1;
    }
    if (state->camera_x + state->viewport.width_tiles > maximum_x) {
        state->camera_x = maximum_x - state->viewport.width_tiles;
        changed = 1;
    }
    if (state->camera_y + state->viewport.height_tiles > maximum_y) {
        state->camera_y = maximum_y - state->viewport.height_tiles;
        changed = 1;
    }
    if (changed && context->event_54_suppress_flood == 1) {
        state->redraw_requested = 1;
    }
    state->camera_y &= ~1;
}

void pharaoh_city_view_grid_offset_to_xy_view(
    const PharaohCityProjectionLookup *projection, int32_t grid_offset,
    int32_t *view_x, int32_t *view_y)
{
    int32_t projected_x, projected_y;
    *view_x = 0;
    *view_y = 0;
    for (projected_x = 0;
         projected_x < PHARAOH_CITY_PROJECTION_WIDTH; ++projected_x) {
        for (projected_y = 0;
             projected_y < PHARAOH_CITY_PROJECTION_HEIGHT; ++projected_y) {
            const int32_t value = projection->cells[
                projection_index(projected_x, projected_y)];
            if ((uint32_t)value < UINT32_C(0x0fff0000) &&
                value == grid_offset) {
                *view_x = projected_y;
                *view_y = projected_x;
                return;
            }
        }
    }
}
