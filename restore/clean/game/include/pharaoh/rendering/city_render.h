#ifndef PHARAOH_RENDERING_CITY_RENDER_H
#define PHARAOH_RENDERING_CITY_RENDER_H

#include "pharaoh/runtime/city_view.h"

typedef struct PharaohCityRenderClip {
    int32_t left, right, top, bottom;
} PharaohCityRenderClip;

typedef struct PharaohCityRenderState {
    int32_t camera_x, camera_y, grid_origin;
    uint32_t last_animation_tick;
    uint32_t advance_animation;
    uint32_t highlight_countdown;
} PharaohCityRenderState;

enum PharaohCityRenderPass {
    PHARAOH_CITY_PASS_BASE, PHARAOH_CITY_PASS_OVERLAY_BASE,
    PHARAOH_CITY_PASS_TOP, PHARAOH_CITY_PASS_OVERLAY_TOP,
    PHARAOH_CITY_PASS_FOOTPRINT, PHARAOH_CITY_PASS_HIGH_WALKERS,
    PHARAOH_CITY_PASS_GRID_VALUES, PHARAOH_CITY_PASS_COUNT
};

typedef struct PharaohCityRenderOperations {
    void *user_data;
    void (*draw_base)(void *user_data);             /* 00430EF0 */
    void (*draw_overlay_base)(void *user_data);     /* 004316F0 */
    void (*draw_top)(void *user_data, int32_t mode); /* 00436B60 */
    void (*draw_overlay_top)(void *user_data);      /* 00439F10 */
    void (*draw_footprint)(void *user_data);        /* 0043B630 */
    void (*draw_high_walkers)(void *user_data, int32_t mode); /* 00439740 */
    void (*draw_grid_values)(void *user_data);      /* 0043E300 */
    /* Null entries retain the shared context. */
    void *pass_user_data[PHARAOH_CITY_PASS_COUNT];
} PharaohCityRenderOperations;

typedef struct PharaohCityRenderContext {
    PharaohCityRenderState *state;
    PharaohCityRenderClip *clip;
    const PharaohCityViewportGeometry *viewport;
    const PharaohCityViewState *view;
    const int32_t *grid_origin;
    const uint32_t *current_tick;
    const int32_t *overlay;
    const uint8_t *alternate_base; /* native006DE515; keep its exact byte test */
    const int32_t *screen_width, *screen_height;
    const PharaohCityRenderOperations *operations;
} PharaohCityRenderContext;

/* Complete native00430D30/004DDDC0 controllers. Rendering endpoints must be
 * supplied by their C owners; these entrypoints do not supply missing passes. */
void pharaoh_city_render(PharaohCityRenderContext *context,
    int32_t camera_x, int32_t camera_y, int32_t grid_origin);
void pharaoh_city_render_current_view(PharaohCityRenderContext *context);

#endif
