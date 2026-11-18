#ifndef PHARAOH_UI_HIGH_SCORES_H
#define PHARAOH_UI_HIGH_SCORES_H

#include <stddef.h>
#include <stdint.h>
#include "pharaoh/scenario/campaign_file.h"

enum {
    PHARAOH_HIGH_SCORE_CAPACITY = 100,
    PHARAOH_HIGH_SCORE_FAMILY_CAPACITY = 32,
    PHARAOH_HIGH_SCORE_MISSION_CAPACITY = 50,
    PHARAOH_HIGH_SCORE_VISIBLE_ROWS = 20
};

typedef struct PharaohHighScoreRecord {
    int32_t score;
    int32_t mission;
    char family[PHARAOH_HIGH_SCORE_FAMILY_CAPACITY];
    int32_t detail[6];
    int32_t difficulty;
    int32_t unknown_44;
    int32_t valid;
} PharaohHighScoreRecord;

typedef struct PharaohHighScoresState {
    PharaohHighScoreRecord records[PHARAOH_HIGH_SCORE_CAPACITY];
    const char (*mission_names)[PHARAOH_HIGH_SCORE_MISSION_CAPACITY];
    int32_t record_count;
    int32_t valid_count;
    int32_t first_visible;
    int32_t origin_x;
    int32_t origin_y;
    int32_t panel_image_base;
    int32_t title_font;
    int32_t normal_font;
    int32_t small_font;
    int32_t empty_font;
    uint8_t *redraw_request;
    uint8_t *current_window_id;
} PharaohHighScoresState;

typedef struct PharaohHighScoresOperations {
    void *user_data;
    int32_t (*read_records)(void *user_data, const char *path,
        void *destination, size_t byte_count);
    void (*draw_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_inner_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_centered_language)(void *user_data, int32_t group,
        int32_t entry, int32_t x, int32_t y, int32_t width,
        int32_t font, int32_t color);
    int32_t (*draw_language)(void *user_data, int32_t group, int32_t entry,
        int32_t x, int32_t y, int32_t font, int32_t color);
    void (*draw_text)(void *user_data, const char *text, int32_t x,
        int32_t y, int32_t font, int32_t color);
    void (*draw_number)(void *user_data, int32_t value, int32_t prefix,
        int32_t x, int32_t y, int32_t font, int32_t color);
    void (*draw_sprite)(void *user_data, int32_t image, int32_t x, int32_t y);
} PharaohHighScoresOperations;

typedef struct PharaohHighScoresInput {
    int32_t cursor_x;
    int32_t cursor_y;
    uint8_t left_pressed;
    uint8_t left_down;
    uint8_t right_released;
} PharaohHighScoresInput;

/* Native 0x004C5880 and 0x004C5920. */
int32_t pharaoh_high_scores_read(PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations, const char *path);
int32_t pharaoh_high_scores_load(PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations, const char *path);
/* Native high-score rows share the campaign mission-name table and count. */
void pharaoh_high_scores_bind_campaign(PharaohHighScoresState *state,
    const PharaohCampaign *campaign);

/* Native 0x004C5400, 0x004C5450, 0x004C54C0 and 0x004C5660. */
void pharaoh_high_scores_scroll_button(PharaohHighScoresState *state,
    int32_t action);
void pharaoh_high_scores_draw_scroll_thumb(
    const PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations);
void pharaoh_high_scores_draw_rows(const PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations);
void pharaoh_high_scores_draw_background(PharaohHighScoresState *state,
    const PharaohHighScoresOperations *operations);

/* Native 0x004C57D0 and the complete state-0x53 input branch. */
int32_t pharaoh_high_scores_drag_scrollbar(PharaohHighScoresState *state,
    const PharaohHighScoresInput *input);
int32_t pharaoh_high_scores_handle_input(PharaohHighScoresState *state,
    const PharaohHighScoresInput *input);

#endif
