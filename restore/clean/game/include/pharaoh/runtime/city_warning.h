#ifndef PHARAOH_RUNTIME_CITY_WARNING_H
#define PHARAOH_RUNTIME_CITY_WARNING_H
#include "pharaoh/city/warning_types.h"
#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/ui/bitmap_font.h"

typedef struct PharaohCityWarningContext {
    const uint8_t *enabled,*construction_enabled;
    const uint32_t *editor_active,*orientation,*time;
    uint32_t *state,*redraw;
    PharaohCityWarning *warnings;
    PharaohTextSelection *selection;
    const PharaohBitmapFont *font;
    const uint8_t *glyph_map;
    uint32_t *current_glyph;
} PharaohCityWarningContext;
void pharaoh_city_warning_post(PharaohCityWarningContext *c,int32_t message); /*004E2970*/
#endif
