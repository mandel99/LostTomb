#ifndef PHARAOH_UI_TOOLTIP_GOODS_H
#define PHARAOH_UI_TOOLTIP_GOODS_H

#include "pharaoh/commerce/luxury_goods.h"
#include "pharaoh/rendering/primitives.h"
#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/ui/bitmap_font.h"
#include "pharaoh/walker/record_queries.h"

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohTooltipGoodsDrawing {
    PharaohGraphicsPrimitives *graphics;
    const PharaohBitmapFont *font;
    PharaohBitmapTextDrawState *text_state;
    PharaohTextSelection *text_selection;
    const uint8_t *glyph_map;
    uint32_t *current_glyph;
    const int32_t *font_id;
    const int32_t *display_mode;
    const int32_t *mouse_x;
    const int32_t *mouse_y;
    int32_t *box_x;
    int32_t *box_y;
    int32_t *box_width;
    int32_t *box_height;
    uint16_t *draw_color;
} PharaohTooltipGoodsDrawing;

typedef struct PharaohWalkerGoodsTooltipContext {
    const PharaohTooltipGoodsDrawing *drawing;
    const int16_t *tile_walker_ids;
    const int32_t *selected_tile;
    const PharaohWalkerQueryRecord *walkers;
} PharaohWalkerGoodsTooltipContext;

typedef struct PharaohLuxuryGoodsTooltipContext {
    const PharaohTooltipGoodsDrawing *drawing;
    const PharaohLuxuryGoodsContext *luxury_goods;
    const uint8_t *comma_text;
} PharaohLuxuryGoodsTooltipContext;

/* Complete typed translations of native 0x004CA9A0 and 0x004CB300. */
void pharaoh_tooltip_draw_walker_goods(
    const PharaohWalkerGoodsTooltipContext *context);
void pharaoh_tooltip_draw_luxury_goods(
    const PharaohLuxuryGoodsTooltipContext *context);

#endif
