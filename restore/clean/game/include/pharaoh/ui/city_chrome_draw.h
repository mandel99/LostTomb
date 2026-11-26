#ifndef PHARAOH_UI_CITY_CHROME_DRAW_H
#define PHARAOH_UI_CITY_CHROME_DRAW_H
#include "pharaoh/ui/city_topbar.h"
#include "pharaoh/ui/ui_text_layout.h"
#include "pharaoh/ui/generic_button_focus.h"
#include "pharaoh/rendering/image_draw.h"

typedef struct PharaohCityChromeDrawContext {
    PharaohImageDrawContext *images;
    const PharaohUiTextLayoutContext *text;
    const PharaohBitmapFont *fonts; /* tags634..641 in native order */
    uint32_t font_count;
    const int32_t *screen_width, *header_image_base, *sprite_group_bases;
    PharaohMenuBarFocusRecord *game_menu, *editor_menu;
    const uint32_t *selected_menu;
    uint8_t *menu_flag_de0, *menu_flag_de1, *force_refresh;
    PharaohImageButtonRecord *menu_button;
    int32_t *draw_x, *draw_y;
    uint32_t *active_image;
} PharaohCityChromeDrawContext;

/* Complete native00425330/00425900/00536670. Font IDs must refer to actual
 * loaded views; UI/text/sprite slots are the corresponding shared state. */
void pharaoh_city_menu_bar_draw(PharaohCityChromeDrawContext *,
    PharaohMenuBarFocusRecord *records, int32_t count);
void pharaoh_city_menu_button_draw(PharaohCityChromeDrawContext *);
void pharaoh_city_header_background_draw(PharaohCityChromeDrawContext *);
/* All seven operations call complete C owners; no default drawing methods. */
void pharaoh_city_topbar_bind_drawing(PharaohCityTopbar *,PharaohCityChromeDrawContext *);
#endif
