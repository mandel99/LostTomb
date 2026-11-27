#ifndef PHARAOH_UI_CITY_DEBUG_GRID_H
#define PHARAOH_UI_CITY_DEBUG_GRID_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/ui/bitmap_font.h"

/* Borrow the actual grids. Signed display loads and saved storage widths are
 * distinct: building_damage aliases the saved WORD sprite/damage grid. */
typedef struct PharaohCityDebugGridValues {
    const int16_t *building_ids,*walker_ids,*building_damage;
    const uint32_t *terrain,*images,*routing_distance,*service_visits;
    const uint8_t *bitfields,*edges,*canals,*random,*sprites,*elevation;
    const uint8_t *old_types,*soldier_strength,*road_network,*road_segment;
    const int8_t *land_citizen,*land_noncitizen,*water,*walls,*desirability;
} PharaohCityDebugGridValues;

typedef struct PharaohCityDebugGridDrawContext {
    const uint32_t *tile; /* 70E488, actual shared city scan */
    const int32_t *mode; /* E38E94, actual settings selector */
    const PharaohCityDebugGridValues *grids;
    const PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    const PharaohBitmapFont *font;
    PharaohBitmapTextDrawState *text;
    char *number_text; /* shared256-byte formatter buffer */
    const int32_t *font_id; /* F56F84, first image of font634 */
    const char *suffix; /* 57C048 */
} PharaohCityDebugGridDrawContext;

/* Whole Native0043E420. All28 selector arms use the original widths, signed
 * magnitudes and real canonical children. Unknown modes and zero values draw
 * nothing as in the executable. Required world/font/renderer owners are
 * caller supplied; this function creates no substitute grids or draw calls. */
void pharaoh_city_debug_draw_grid_value(
    const PharaohCityDebugGridDrawContext *,int32_t x,int32_t y);
int32_t pharaoh_city_debug_grid_bound_for_current_mode(
    const PharaohCityDebugGridDrawContext *);

#endif
