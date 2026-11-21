#ifndef PHARAOH_UI_BRIEFING_H
#define PHARAOH_UI_BRIEFING_H

#include <stdint.h>

typedef struct PharaohBriefingPoint {
    int32_t x;
    int32_t y;
} PharaohBriefingPoint;

typedef struct PharaohBriefingScenario {
    int32_t population_enabled;
    int32_t population;
    int32_t houses;
    int32_t houses_level;
    int32_t rating_values[4];
    uint8_t rating_enabled[4];
} PharaohBriefingScenario;

typedef struct PharaohBriefingState {
    uint8_t started;
    uint8_t current_window_id;
    uint8_t redraw_requested;
    uint8_t scroll_up_enabled;
    uint8_t scroll_down_enabled;
    int32_t origin_x;
    int32_t origin_y;
    int32_t window_width;
    int32_t window_height;
    int32_t difficulty;
    int32_t line_count;
    int32_t viewport_lines;
    int32_t scroll_position;
    int32_t scroll_maximum;
} PharaohBriefingState;

typedef struct PharaohBriefingMessage {
    const char *title;
    const char *subtitle;
    const char *body;
} PharaohBriefingMessage;

typedef struct PharaohBriefingOperations {
    void *user_data;
    void (*load_selected_mission)(void *user_data);
    void (*draw_background)(void *user_data, int32_t image_group,
        int32_t image_offset, int32_t x, int32_t y);
    void (*draw_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_inner_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_label_frame)(void *user_data, int32_t style, int32_t x,
        int32_t y, int32_t width_blocks, int32_t immediate,
        int32_t variant);
    int32_t (*draw_language)(void *user_data, int32_t group, int32_t entry,
        int32_t x, int32_t y, int32_t font_group, int32_t color);
    int32_t (*draw_number)(void *user_data, int32_t value, int32_t prefix,
        int32_t x, int32_t y, int32_t font_group, int32_t color);
    void (*draw_text)(void *user_data, const char *text, int32_t x,
        int32_t y, int32_t font_group, int32_t color);
    int32_t (*draw_rich_text)(void *user_data, const char *text, int32_t x,
        int32_t y, int32_t width, int32_t measure_only,
        int32_t large_line, int32_t scroll_position,
        int32_t viewport_lines);
    const char *(*tutorial_hint)(void *user_data);
} PharaohBriefingOperations;

typedef struct PharaohBriefingInputOperations {
    void *user_data;
    void (*set_scroll_button_tooltip)(void *user_data, int32_t direction,
        int32_t x, int32_t y, int32_t tooltip_id);
    int32_t (*handle_scroll_button)(void *user_data, int32_t direction,
        int32_t x, int32_t y);
    void (*handle_scrollbar)(void *user_data);
} PharaohBriefingInputOperations;

/* Complete typed rendering controller for native 0x0041A180. The operation
   boundary owns the already-separated native text, panel and mission loaders. */
void pharaoh_briefing_draw(PharaohBriefingState *state,
    const PharaohBriefingScenario *scenario,
    const PharaohBriefingMessage *message, int32_t campaign_has_choices,
    const PharaohBriefingOperations *operations);

/* Complete typed input controller for native 0x00418FE0. */
void pharaoh_briefing_handle_input(int32_t origin_x, int32_t origin_y,
    const PharaohBriefingInputOperations *operations);

#endif
