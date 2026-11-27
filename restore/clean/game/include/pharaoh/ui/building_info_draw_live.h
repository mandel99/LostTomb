#ifndef PHARAOH_UI_BUILDING_INFO_DRAW_LIVE_H
#define PHARAOH_UI_BUILDING_INFO_DRAW_LIVE_H

#include "pharaoh/ui/building_info_live.h"
#include "pharaoh/ui/button_border.h"
#include "pharaoh/ui/ui_text_layout.h"
#include "pharaoh/city/labor.h"

typedef struct PharaohInfoLayoutDrawContext {
    const PharaohUiLayoutContext *layout;
    const uint8_t *window_id;
    const int32_t *window_seven_layout_base;
    PharaohLayoutHighlightState *highlight;
    const PharaohButtonBorderContext *border;
    const PharaohPanelFrameNativeContext *inner_panel;
    const PharaohPanelFrameNativeContext *outer_panel;
} PharaohInfoLayoutDrawContext;

/* Complete4D63D0/4D65B0. layout/highlight must share the actual initialized
 * catalogue and edit/marked cells used by the existing UI text/layout owners.
 * Inner image_base is F56844; outer image_base is F5683C. */
void pharaoh_info_layout_draw_border(const PharaohInfoLayoutDrawContext *,
    int32_t style,int32_t x,int32_t y,int32_t width,int32_t height,
    int32_t flags,int32_t origin_x,int32_t origin_y);
void pharaoh_info_layout_draw_inner_panel(const PharaohInfoLayoutDrawContext *,
    int32_t x,int32_t y,int32_t width_blocks,int32_t height_blocks,
    int32_t origin_x,int32_t origin_y);

typedef struct PharaohInfoSquareDrawContext {
    PharaohImageDrawContext *drawing;
    uint32_t *current_image;
    int32_t *draw_x,*draw_y;
    uint8_t *ui_redraw;
} PharaohInfoSquareDrawContext;
/* Complete4256A0. The saved square records retain their24-byte native layout. */
void pharaoh_info_square_buttons_draw(const PharaohInfoSquareDrawContext *,
    int32_t x,int32_t y,const PharaohSquareButtonRecord *,int32_t count);

/* Complete40D1B0. logical left_down is nativeE38E00; cursor coordinates are
 * E38E34/E38E30. The selected building and order scroll remain in actual pools. */
int32_t pharaoh_info_storage_scrollbar(PharaohBuildingInfoLiveContext *,
    uint8_t left_down,int32_t cursor_x,int32_t cursor_y);

/* Complete4D6160. The wrapped context is a template borrowing the shared
 * selection/text/glyph/buffer state. Its font view is replaced locally with
 * font, matching the supplied native font_id; no renderer state is copied. */
void pharaoh_info_ui_draw_multiline(const PharaohInfoLayoutDrawContext *,
    const PharaohBitmapTextCatalogWrappedNativeContext *,const PharaohBitmapFont *,
    int32_t group,int32_t entry,int32_t x,int32_t y,int32_t width,
    int32_t font_id,uint16_t color,int32_t origin_x,int32_t origin_y);
/* Complete4CCC30. suffix borrows the actual7027D4 number-suffix bytes. */
void pharaoh_info_text_draw_amount(const PharaohUiTextLayoutContext *,
    const PharaohBitmapFont *,const char *suffix,int32_t group,int32_t entry,
    int32_t value,int32_t x,int32_t y,int32_t font_id,uint16_t color);

typedef struct PharaohStorageInfoDrawContext {
    PharaohBuildingInfoLiveContext *ui;
    const PharaohInfoLayoutDrawContext *layout;
    const PharaohUiTextLayoutContext *text;
    const PharaohBitmapTextCatalogWrappedNativeContext *wrapped;
    const PharaohBitmapFont *font_black,*font_green,*font_red,*font_white,*font_title;
    const PharaohBuildingModelRecord *models; /* Actual705EA4, stride40 */
    const PharaohLaborCategory *labor; /* selected player's EA439C, stride20 */
    const int32_t *city_population; /* selected player's EA1894 */
    const int32_t *kingdom_supplies_grain; /* actual DWORD784BBC */
    const int32_t *context_icons; /* F5684C */
    const char *number_suffix; /* 7027D4 */
    const char *employee_suffix; /* 57C048 */
} PharaohStorageInfoDrawContext;

/* Whole normal500A40/5014A0/501FD0 and their real188-instruction4FC5C0 child.
 * ui.text_width aliases text.drawing.cursor_x(E20778). Each font asset view
 * matches the corresponding live ui font ID. No partial panel callbacks. */
void pharaoh_info_draw_bazaar(const PharaohStorageInfoDrawContext *);
void pharaoh_info_draw_granary(const PharaohStorageInfoDrawContext *);
void pharaoh_info_draw_storage_yard(const PharaohStorageInfoDrawContext *);
void pharaoh_info_draw_employment(const PharaohStorageInfoDrawContext *,int32_t y);

#endif
