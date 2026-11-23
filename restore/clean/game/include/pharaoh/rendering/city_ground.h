#ifndef PHARAOH_RENDERING_CITY_GROUND_H
#define PHARAOH_RENDERING_CITY_GROUND_H
#include "pharaoh/rendering/city_render.h"
#include "pharaoh/runtime/city_sound.h"

/* Native scan prefix70E440..70E490. Projection cells are a separate buffer. */
typedef struct PharaohCityGroundScan {
    uint32_t tile_y[4], tile_x[4]; /* multiples4,3,2,1 */
    uint32_t visible_rows, visible_columns, reserved_28[3];
    uint32_t camera_row, camera_column, row, column, reserved_44;
    uint32_t tile, start_y, start_x;
} PharaohCityGroundScan;

typedef struct PharaohCityGroundOperations {
    void *user_data;
    void (*draw_image)(void *,int32_t image,int32_t x,int32_t y); /*0053E590 */
    void (*draw_top)(void *,int32_t image,int32_t x,int32_t y); /*0053F250 */
    void (*draw_overlay_27)(void *,int32_t x,int32_t y,uint32_t mode); /*00433760 */
    void (*draw_overlay_28)(void *,int32_t x,int32_t y); /*00434E10 */
    void (*draw_resource_overlay)(void *,int32_t x,int32_t y,uint32_t a,uint32_t b); /*00434AA0 */
    void (*draw_overlay_45)(void *,int32_t x,int32_t y); /*00431CE0 */
    void (*draw_overlay_51)(void *,int32_t x,int32_t y); /*00432620 */
    void (*draw_building_overlay)(void *,int32_t building,int32_t x,int32_t y,uint32_t mode); /*00432D30 */
    void *image_user_data; /* NULL uses user_data for the image callback. */
    void *top_user_data; /* NULL uses user_data for the top callback. */
} PharaohCityGroundOperations;

typedef struct PharaohCityGroundContext {
    PharaohCityGroundScan *scan;
    const uint32_t *cells, *neighbor_cells; /*457*231 entries */
    const int16_t *building_ids, *walker_ids;
    uint32_t *images;
    const uint32_t *terrain;
    const uint8_t *footprints, *property_flags;
    PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    uint32_t *current_image, *ambient_scores, *ambient_total;
    uint16_t *tint;
    const uint32_t *marked_tile, *advance_animation, *debug_invalid, *overlay;
    uint32_t *marked_x, *marked_y;
    const uint8_t *alternate_overlay;
    const uint32_t *water_image, *water_variant_a, *water_variant_b;
    const uint32_t *cleared_image, *royal_tomb_image;
    const int32_t *sweep_countdown, *sweep_start;
    const int32_t *building_sound_ids, *sound_suppressed;
    PharaohCitySoundViewRecord *sound_views;
    const PharaohCityGroundOperations *operations;
} PharaohCityGroundContext;

void pharaoh_city_draw_ground(PharaohCityGroundContext *c); /*00430EF0 */
void pharaoh_city_draw_overlay_ground(PharaohCityGroundContext *c); /*004316F0 */
void pharaoh_city_render_bind_ground(PharaohCityRenderOperations *operations,
    PharaohCityGroundContext *ground);
#endif
