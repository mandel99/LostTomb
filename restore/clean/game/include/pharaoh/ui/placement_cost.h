#ifndef PHARAOH_UI_PLACEMENT_COST_H
#define PHARAOH_UI_PLACEMENT_COST_H
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/rendering/primitives.h"
#include "pharaoh/scenario/scenario_init_custom.h"
#include "pharaoh/ui/bitmap_font.h"

typedef struct PharaohPlacementCostContext {
    const PharaohCityViewContext *city;
    const PharaohCityMapBounds *bounds;
    const int32_t *active, *tile_x, *tile_y, *hidden;
    int32_t *cost;
    const PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player, *screen_width, *screen_height, *font_image;
    PharaohSpriteClipState *clip; /* actual bounds shared by the glyph provider */
    const PharaohBitmapFont *font;
    PharaohBitmapTextDrawState *text;
    char *number_text; /* actual native 256-byte formatter buffer */
} PharaohPlacementCostContext;

/* Complete native0043E1A0. Clip/font/text storage must be the live shared state. */
void pharaoh_placement_cost_draw(const PharaohPlacementCostContext *context);
#endif
