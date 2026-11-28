#ifndef PHARAOH_UI_TOOLTIP_TEXT_PANELS_H
#define PHARAOH_UI_TOOLTIP_TEXT_PANELS_H

#include "pharaoh/ui/tooltip_goods.h"
#include "pharaoh/ui/rich_text.h"

#include <stdint.h>

typedef struct PharaohComposedTooltipContext {
    const PharaohTooltipGoodsDrawing *drawing;
    PharaohRichTextState *rich_text;
    const PharaohRichTextOperations *rich_text_operations;
    const uint8_t *text;
} PharaohComposedTooltipContext;

typedef struct PharaohSenateTooltipValues {
    int32_t unemployment;
    int32_t culture;
    int32_t prosperity;
    int32_t monument;
    int32_t kingdom;
    int32_t tax;
} PharaohSenateTooltipValues;

typedef struct PharaohSenateTooltipContext {
    const PharaohTooltipGoodsDrawing *drawing;
    const PharaohSenateTooltipValues *values;
    char *number_scratch;
    const char *percent_suffix;
    const char *empty_suffix;
} PharaohSenateTooltipContext;

/* Complete typed translations of native 0x004CAF10 and 0x004CB0B0. */
void pharaoh_tooltip_draw_composed_text_box(
    const PharaohComposedTooltipContext *context);
void pharaoh_tooltip_draw_senate(
    const PharaohSenateTooltipContext *context);

#endif
