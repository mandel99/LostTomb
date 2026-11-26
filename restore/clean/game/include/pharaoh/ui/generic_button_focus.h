#ifndef PHARAOH_UI_GENERIC_BUTTON_FOCUS_H
#define PHARAOH_UI_GENERIC_BUTTON_FOCUS_H

#include <stdint.h>

#include "pharaoh/ui/main_menu.h"

typedef struct PharaohGenericButtonFocusState {
    int32_t hit_mouse_y;
    int32_t hit_mouse_x;
} PharaohGenericButtonFocusState;

typedef struct PharaohImageButtonRecord {
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    int16_t trigger;
    int16_t image_group;
    int16_t image_offset;
    uint8_t reserved_0e[10];
    uint8_t enabled;
    uint8_t activation_ticks;
    uint8_t pressed;
    uint8_t animation;
    uint8_t reserved_1c[8];
} PharaohImageButtonRecord;

typedef struct PharaohImageButtonDrawState {
    const int32_t *sprite_group_bases;
    int32_t *draw_x;
    int32_t *draw_y;
    int32_t *active_sprite_image;
    uint8_t *ui_redraw;
    const uint8_t *window_redraw_requested; /* nativeE38DCA is a BYTE */
    uint8_t *force_refresh;
    void (*draw_sprite)(void *user_data, int32_t image,
        int32_t x, int32_t y);
    void *user_data;
} PharaohImageButtonDrawState;

typedef struct PharaohSquareButtonRecord {
    int16_t x;
    int16_t y;
    int16_t image;
    int16_t size;
    uint32_t callback_token;
    uint8_t pressed;
    uint8_t countdown;
    uint8_t repeat;
    uint8_t trigger;
    uint32_t primary_parameter;
    uint32_t secondary_parameter;
} PharaohSquareButtonRecord;

typedef uint32_t (*PharaohSquareButtonClock)(void *user_data);
typedef void (*PharaohSquareButtonDispatch)(
    void *user_data, uint32_t callback_token);

typedef struct PharaohSquareButtonInputState {
    uint32_t last_repeat_tick;
    int32_t cursor_x;
    int32_t cursor_y;
    int32_t editor_map_preview;
    uint8_t current_window_id;
    uint32_t editor_mode;
    uint8_t left_went_down;
    uint8_t left_clicked;
    uint8_t left_down;
    const uint8_t *repeat_schedule;
    int32_t focused_building;
    uint32_t primary_parameter;
    uint32_t secondary_parameter;
    uint32_t repeat_value;
    PharaohSquareButtonClock clock;
    PharaohSquareButtonDispatch dispatch;
    void *user_data;
} PharaohSquareButtonInputState;

typedef struct PharaohMenuBarFocusRecord {
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t reserved_06;
    uint8_t reserved_08[8];
} PharaohMenuBarFocusRecord;

typedef struct PharaohButtonHitState {
    int32_t hit_mouse_x;
    int32_t hit_mouse_y;
    int32_t focused_id;
} PharaohButtonHitState;

/* Original entry 0x00426450. Returns the first hit button as a one-based
   index and updates hit_mouse_y/x only when a rectangle contains the cursor. */
int32_t pharaoh_generic_buttons_get_focus(
    int32_t origin_x, int32_t origin_y,
    const PharaohMenuRect *rectangles, int32_t rectangle_count,
    int32_t cursor_x, int32_t cursor_y,
    PharaohGenericButtonFocusState *focus_state);
void pharaoh_image_buttons_clear_radio(
    PharaohImageButtonRecord *buttons, int32_t button_count,
    uint8_t *redraw_requested);
void pharaoh_image_buttons_decay(
    PharaohImageButtonRecord *buttons, int32_t button_count,
    uint8_t *redraw_requested);
void pharaoh_image_buttons_draw(int32_t origin_x, int32_t origin_y,
    PharaohImageButtonRecord *buttons, int32_t button_count,
    PharaohImageButtonDrawState *state);
void pharaoh_image_buttons_set_tooltip(int32_t origin_x, int32_t origin_y,
    const PharaohImageButtonRecord *buttons, int32_t button_count,
    int32_t first_tooltip_id, int32_t cursor_x, int32_t cursor_y,
    PharaohButtonHitState *hit_state);
void pharaoh_square_buttons_get_focus(int32_t origin_x, int32_t origin_y,
    const PharaohSquareButtonRecord *buttons, int32_t button_count,
    int32_t first_button_id, int32_t cursor_x, int32_t cursor_y,
    PharaohButtonHitState *hit_state);
int32_t pharaoh_square_buttons_handle_mouse(
    int32_t origin_x, int32_t origin_y,
    PharaohSquareButtonRecord *buttons, int32_t button_count,
    PharaohSquareButtonInputState *state);
int32_t pharaoh_menu_bar_focus(const PharaohMenuBarFocusRecord *entries,
    int32_t entry_count, int32_t cursor_x, int32_t cursor_y);
void pharaoh_building_info_draw_palace_foreground(
    int32_t building_info_x, int32_t building_info_y,
    PharaohSquareButtonRecord tax_arrow_buttons[2],
    PharaohButtonHitState *hit_state,
    PharaohSquareButtonInputState *input_state);
void pharaoh_building_info_draw_advisor_buttons(
    int32_t building_info_x, int32_t building_info_y,
    int32_t building_info_height, const int32_t shown[15],
    const uint8_t tutorial_flags[15],
    PharaohImageButtonRecord *const tutorial_buttons[15],
    PharaohImageButtonDrawState *draw_state);

#endif
