#ifndef PHARAOH_UI_MESSAGE_BOX_H
#define PHARAOH_UI_MESSAGE_BOX_H

#include <stdint.h>

enum { PHARAOH_MESSAGE_BOX_MAX_BUTTONS = 3 };

typedef struct PharaohMessageBoxButton {
    int16_t left, top, right, bottom;
    int32_t label;
    int32_t result;
} PharaohMessageBoxButton;

typedef struct PharaohMessageBoxState {
    int32_t screen_width, screen_height;
    int32_t origin_x, origin_y;
    int32_t title_font, body_font, title_draw_font;
    int32_t button_count;
    int32_t focused_button;
    uint8_t activated;
    int32_t selected_index;
    PharaohMessageBoxButton buttons[PHARAOH_MESSAGE_BOX_MAX_BUTTONS];
} PharaohMessageBoxState;

typedef struct PharaohMessageBoxOperations {
    void *user_data;
    int32_t (*measure_text)(void *user_data, const char *text, int32_t font);
    void (*draw_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_inner_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_centered_text)(void *user_data, const char *text,
        int32_t x, int32_t y, int32_t width, int32_t font);
    void (*draw_button_label)(void *user_data, int32_t group,
        int32_t entry, int32_t x, int32_t y, int32_t width, int32_t font);
    void (*draw_button)(void *user_data, int32_t x, int32_t y,
        int32_t width, int32_t height, uint32_t color);
} PharaohMessageBoxOperations;

typedef struct PharaohMessageBoxModalState {
    PharaohMessageBoxState *box;
    uint8_t *current_window_id;
    uint8_t *redraw_request;
    uint8_t *post_close_requested;
    uint32_t *display_unavailable;
    uint32_t *display_ready;
} PharaohMessageBoxModalState;

typedef struct PharaohMessageBoxModalOperations {
    void *user_data;
    int32_t (*has_message)(void *user_data);
    int32_t (*get_translate_dispatch)(void *user_data);
    void (*wait_message)(void *user_data);
    int32_t (*run_frame)(void *user_data, const char *title,
        const char *body, uint32_t type);
    void (*post_close)(void *user_data);
} PharaohMessageBoxModalOperations;

/* Native 0x00538920 and 0x00538940. */
void pharaoh_message_box_select(PharaohMessageBoxState *state,
    int32_t selected_index);
int32_t pharaoh_message_box_draw(PharaohMessageBoxState *state,
    const PharaohMessageBoxOperations *operations,
    const char *title, const char *body, uint32_t type);

/* Native modal loop 0x00538CD0. */
int32_t pharaoh_message_box_show(PharaohMessageBoxModalState *state,
    const PharaohMessageBoxModalOperations *operations,
    const char *title, const char *body, uint32_t type);

#endif
