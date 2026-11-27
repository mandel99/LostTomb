#ifndef PHARAOH_UI_CITY_PENDING_ACTIONS_H
#define PHARAOH_UI_CITY_PENDING_ACTIONS_H

#include "pharaoh/ui/city_ingame_dispatch.h"
#include "pharaoh/ui/sidebar_background.h"
#include "pharaoh/ui/sidebar_content.h"
#include "pharaoh/ui/ui_text_layout.h"
#include "pharaoh/rendering/city_render.h"

typedef struct PharaohCityProblemFormation {
    uint8_t state, reserved_01[0x2e - 1];
    int16_t x, y;
    uint8_t reserved_32[0x42 - 0x32], flag_42;
    uint8_t reserved_43[0x69 - 0x43], flag_69;
    uint8_t reserved_6a[0x88 - 0x6a];
    int16_t problem_key;
    uint8_t reserved_8a[0x90 - 0x8a];
} PharaohCityProblemFormation;

typedef struct PharaohCityPendingMessageContext {
    PharaohMenuMessageRecord *messages, *swap; /* E95228[1000],E951F8 */
    int32_t *selected_message, *message_count; /* E951E4/EC */
    int32_t *scroll_limit, *scroll_index; /* E9511C/20 */
    const PharaohCityMessageMediaLayout *layouts; /* E209B8,80-byte records */
    const PharaohCityProblemFormation *formations; /* BCB9C8,50 records */
    const int32_t *grid_origin, *current_year; /* BCA2D8/78D7BC */
    uint32_t *problem_index, *problem_count, *last_problem_clock; /* E951D4/D8/D0 */
    const uint32_t *button_parameter; /* D3B198 */
    const PharaohGameTickGateOperations *clock;
    PharaohCityViewContext *view;
    int32_t *focus_x, *focus_y;
    uint8_t *current_window, *redraw;
} PharaohCityPendingMessageContext;

uint32_t pharaoh_city_problem_group_tile(const PharaohCityPendingMessageContext *,
    int32_t key); /* 4B80B0 */
void pharaoh_city_messages_sort(const PharaohCityPendingMessageContext *); /* 4E1640 */
void pharaoh_city_message_list_open(const PharaohCityPendingMessageContext *); /* 4E1620 */
void pharaoh_city_sidebar_messages(const PharaohCityPendingMessageContext *); /* 406020 */
void pharaoh_city_problem_locate(const PharaohCityPendingMessageContext *); /* 4E23E0 */
void pharaoh_city_sidebar_problem(const PharaohCityPendingMessageContext *); /* 406030 */

typedef struct PharaohCityBuildPopupDrawContext {
    PharaohBuildMenuContext *menu;
    const int32_t *expanded_x, *collapsed_x, *collapsed;
    const int32_t *count, *y_offset, *focused_button;
    /* Native784CB0 is read as DWORD for nonzero animal rows and BYTE for row0. */
    const int32_t *climate;
    const int32_t *normal_font_id, *focused_font_id; /* F56F8C/F56F94 */
    const PharaohBitmapFont *normal_font, *focused_font;
    const int16_t *goal_definition_ids; /* 784DE0 */
    const PharaohBuildingModelRecord *models;
    const PharaohUiTextLayoutContext *text;
    const PharaohLabelRenderer *labels;
    const int32_t *origin_x, *origin_y;
    const uint8_t *window_id;
    const int32_t *window_seven_layout_base;
    int32_t *menu_animation; /* F53F30, real animation slot6 */
    const char *money_postfix;
} PharaohCityBuildPopupDrawContext;

/* Complete 4D6480 uses the actual layout file, label strips and highlight
   drawing bindings already held by text/labels; it creates no renderer. */
void pharaoh_city_label_draw_animated_ui(const PharaohCityBuildPopupDrawContext *,
    int32_t id, int32_t x, int32_t y, int32_t width_blocks,
    int32_t force, int32_t type, int32_t origin_x, int32_t origin_y);
void pharaoh_city_build_popup_draw_foreground(
    const PharaohCityBuildPopupDrawContext *); /* 520000 */

typedef struct PharaohCityPendingBackgroundContext {
    const PharaohSidebarBackgroundState *sidebar;
    const PharaohSidebarBackgroundOperations *sidebar_operations;
    PharaohCityRenderContext *city;
    const PharaohCityBuildPopupDrawContext *popup;
    uint8_t *current_window, *redraw;
    const uint32_t *modal_active, *modal_return_window; /* D3A6E4/D3A6C4 */
} PharaohCityPendingBackgroundContext;

void pharaoh_city_build_popup_draw_background(
    const PharaohCityPendingBackgroundContext *); /* 51FFD0 */
/* Actual 4D71C0 prefix and windows1,2,6,18. Returns0 for a different selected
   window, leaving its dispatch to that real owner; not a full4D71C0 owner. */
uint8_t pharaoh_city_pending_draw_background(
    const PharaohCityPendingBackgroundContext *, int32_t force);

typedef struct PharaohCitySidebarSlideContext {
    const int32_t *frame_duration, *screen_width; /* E39104/E38DB0 */
    uint32_t *step; /* F53F7C, advanced value retained even past47 */
    int32_t *collapsed; /* F53F78 */
    const int32_t *offsets; /* all48 pinned DWORDs at5F8260 */
    const int32_t *collapsed_x, *expanded_x, *game_image_base;
    uint8_t *current_window, *ui_redraw;
    PharaohCityViewContext *view;
    PharaohCityRenderContext *city;
    PharaohImageDrawContext *images;
    PharaohImageButtonDrawState *button_draw;
    PharaohSidebarContent *content;
    PharaohGraphicsPrimitives *graphics;
    const PharaohCityIngameButtonTables *buttons;
    const PharaohCityPendingBackgroundContext *background;
} PharaohCitySidebarSlideContext;

/* Actual51FD60 algorithm. The only boundary result0 is an unsupported
   modal-selected4D71C0 background at the terminal step; it is explicit,
   never a callback stub. Ordinary Window2 producers finish into Window1. */
uint8_t pharaoh_city_sidebar_slide(const PharaohCitySidebarSlideContext *);

/* Exact4D8310 common input prefix and Window2's scroll-only branch.
   The already bound in-game context supplies the same real shared fields.
   Other windows are left untouched for their own dispatcher branch. */
void pharaoh_city_sidebar_slide_input(const PharaohCityIngameDispatchContext *);

#endif
