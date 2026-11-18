#ifndef PHARAOH_RUNTIME_CITY_VIEW_H
#define PHARAOH_RUNTIME_CITY_VIEW_H

#include <stdint.h>

enum {
    PHARAOH_CITY_MAP_WIDTH = 228,
    PHARAOH_CITY_MAP_HEIGHT = 228,
    PHARAOH_CITY_MAP_CELL_COUNT =
        PHARAOH_CITY_MAP_WIDTH * PHARAOH_CITY_MAP_HEIGHT,
    PHARAOH_CITY_PROJECTION_WIDTH = 457,
    PHARAOH_CITY_PROJECTION_HEIGHT = 231,
    PHARAOH_CITY_PROJECTION_CELL_COUNT =
        PHARAOH_CITY_PROJECTION_WIDTH * PHARAOH_CITY_PROJECTION_HEIGHT,
    PHARAOH_CITY_TILE_WIDTH = 58,
    PHARAOH_CITY_TILE_HEIGHT = 30,
    PHARAOH_CITY_VIEW_TOP = 30
};

typedef struct PharaohCityMapBounds {
    int32_t minimum_x;
    int32_t minimum_y;
    int32_t maximum_x;
    int32_t maximum_y;
    int32_t center_x;
    int32_t center_y;
} PharaohCityMapBounds;

typedef struct PharaohCityViewportGeometry {
    int32_t map_width;
    int32_t map_height;
    int32_t screen_x;
    int32_t screen_y;
    int32_t width_tiles;
    int32_t height_tiles;
    int32_t maximum_camera_x;
    int32_t screen_right;
    int32_t screen_bottom;
    int32_t tile_width;
    int32_t tile_half_width;
    int32_t tile_width_and_half;
    int32_t tile_double_width;
    int32_t tile_height;
    int32_t tile_half_height;
    int32_t tile_height_and_half;
    int32_t tile_double_height;
} PharaohCityViewportGeometry;

typedef struct PharaohCityProjectionLookup {
    int32_t map_width;
    int32_t map_height;
    int32_t map_origin_x;
    int32_t map_origin_y;
    int32_t orientation;
    int32_t projected_minimum_x;
    int32_t projected_minimum_y;
    int32_t projected_maximum_x;
    int32_t projected_maximum_y;
    int32_t cells[PHARAOH_CITY_PROJECTION_CELL_COUNT];
} PharaohCityProjectionLookup;

typedef struct PharaohCityViewState {
    PharaohCityViewportGeometry viewport;
    int32_t visible_minimum_x;
    int32_t visible_minimum_y;
    int32_t visible_maximum_x;
    int32_t visible_maximum_y;
    int32_t camera_x;
    int32_t camera_y;
    uint8_t redraw_requested;
} PharaohCityViewState;

typedef struct PharaohCityViewContext {
    PharaohCityViewState *state;
    PharaohCityProjectionLookup *projection;
    const int32_t *map_image_grid;
    const PharaohCityMapBounds *map_bounds;
    int32_t invalid_tile_offset_seed;
    int32_t screen_width;
    int32_t debug_extra_camera_bounds_enabled;
    int32_t event_54_suppress_flood;
} PharaohCityViewContext;

void pharaoh_city_view_set_viewport(PharaohCityViewState *state,
                                    int32_t map_width, int32_t map_height,
                                    int32_t screen_x, int32_t screen_y,
                                    int32_t width_tiles,
                                    int32_t height_tiles,
                                    int32_t tile_width,
                                    int32_t tile_height);
void pharaoh_city_view_projection_reset(PharaohCityViewContext *context);
void pharaoh_city_view_calculate_lookup(PharaohCityViewContext *context,
                                        int32_t orientation);
int32_t pharaoh_city_view_get_viewport_width_tiles(
    const PharaohCityViewState *state);
int32_t pharaoh_city_view_get_viewport_height_tiles(
    const PharaohCityViewState *state);
int32_t pharaoh_city_view_width_tiles(const PharaohCityViewState *state);
int32_t pharaoh_city_view_height_tiles(const PharaohCityViewState *state);
int32_t pharaoh_city_view_grid_offset_is_outside_map(
    const PharaohCityViewContext *context, int32_t grid_offset);
void pharaoh_city_view_set_viewport_without_sidebar(
    PharaohCityViewContext *context);
void pharaoh_city_view_set_viewport_with_sidebar(
    PharaohCityViewContext *context);
void pharaoh_city_view_set_editor_viewport(PharaohCityViewContext *context);
void pharaoh_city_view_calculate_map_bounds(PharaohCityViewContext *context);
void pharaoh_city_view_check_camera_boundaries(PharaohCityViewContext *context);

#endif
